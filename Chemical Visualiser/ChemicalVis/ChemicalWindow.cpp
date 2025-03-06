#include "ChemicalWindow.h"
#include <iostream>


static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	auto* window = static_cast<ChemicalFetchWindow*>(data->UserData);
	window->setBuff(data->Buf, data->BufSize);

	//strcpy_s(data->Buf, data->BufSize, static_cast<std::string*>(data->UserData)->c_str());

	return 0;
}


void ChemicalFetchWindow::Display()
{
	//runs at the start once
	if (init)
	{
		//Get IO
		io = &ImGui::GetIO();
		//Set the position of the window
		ImGui::SetNextWindowPos(ImVec2(0, 20));

		init = false;
	}
	
	//create the window
	if (ImGui::Begin("Chemical Input Window", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		//Display the input
		DisplayContent();
	}
	//finish creating the window
	ImGui::End();
}

void ChemicalFetchWindow::setBuff(char* buf, int size)
{
	if (queueTab)
	{
		queueTab = false;
		strcpy_s(buf, size, inpValue.c_str());
	}
}

void ChemicalFetchWindow::DisplayContent()
{
	if (ImGui::InputText("##Chemical Name", inpBuf, IM_ARRAYSIZE(inpBuf), ImGuiInputTextFlags_CallbackEdit, InputTextCallback, this))
	{
		//if there is a change in the user input make a autocomplete request
		CIH::queueAutoComplete();
	}
	if (ImGui::IsItemActive() && false)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_Tab, false) && AutoCompleteOptionArr.size() > 0) {
			queueTab = true;
			selectedAutoCompleteOption++;
			if (selectedAutoCompleteOption >= AutoCompleteOptionArr.size()) selectedAutoCompleteOption = 0;
			std::cout << selectedAutoCompleteOption << ", " << AutoCompleteOptionArr.size() << "\n";
			
			inpValue = AutoCompleteOptionArr[selectedAutoCompleteOption];
		}
		else
		{
			inpValue = std::string(inpBuf);
		}
		
	}
	else
	{
		selectedAutoCompleteOption = -1;
	}


	// if the user pressed enter and their is an input get the data about chemical if possible 
	// TODO : allow the enter key to be changed in a settings menu
	bool entered = ImGui::IsItemDeactivatedAfterEdit() && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter) && std::string(inpBuf).size() > 0;

	// Position of the input box
	ImVec2 chemicalInputPos = ImGui::GetCursorScreenPos();
	// is the input box in use?
	chemicalInputIsActive = ImGui::IsItemActive();

	// If the width of the window is too small put the ontop of each other
	if (ImGui::GetWindowWidth() > 150) ImGui::SameLine();

	//if name is being input do autocomplete checks
	if (selectedInputType == 0) NameInput(chemicalInputPos);

	// Set the width of the next widget
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 1.0f); 
	
	// The user can select the type of input they want to use: NAME, CID, SMILES
	if (ImGui::Combo("##Input Type", &selectedInputType, inputTypes, IM_ARRAYSIZE(inputTypes)))
	{
		CIH::queueAutoComplete();
	}
	// Reset widget width to default settings
	ImGui::PopItemWidth(); 

	// Get the data about the chemical when pressed
	if (ImGui::Button("Search") || entered)
	{
		// Get the data about a chemical from pubchem
		chemicalData = CIH::GetData(inpBuf, selectedInputType);
		if (CIH::ValidateData(chemicalData.data) == -1)
		{
			inputError = "Invalid Input"; //state the error to the user
		}
		else
		{
			// Reset error to display nothing as the current chemical is likely valid
			inputError = "";
			
			// Create the chemical
			// And add it to the chemicals list
			globalClass::chemicals.push_back(Chemical(chemicalData.data, chemicalData.conformers));
		}
	}
	chemicalData = { "", "" };


	// Autocomplete window position offset to not cover the button
	acOffset = ImGui::GetItemRectSize().x;

	// If there is an error to be displayed, display it to user
	if (inputError.size() > 0)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), (std::string("ERROR: ") + inputError).c_str());
	}
}

void ChemicalFetchWindow::NameInput(ImVec2& chemicalInputPos)
{
	// Show the autocomplete options in a menu
	DisplayAutoCompleteOptions(chemicalInputPos);

	// If there is a request queued
	// Make the request
	CIH::makeAutoCompleteRequest(std::string(inpBuf));

	// Get the data from the ongoing autocomplete request 
	if (CIH::checkRequestProgress() == 1)
	{
		autoCompleteOptions = CIH::getRequestData();
	}
}

void ChemicalFetchWindow::DisplayAutoCompleteOptions(ImVec2& chemicalInputPos)
{
	// If there is nothing to output or the textbox/popup is selected
	// Don't display the autocomplete options
	if (autoCompleteOptions.size() <= 0 || (!chemicalInputIsActive && !autoCompleteActive)) return;
	
	// Convert the string to a JSON object
	nlohmann::json optionsJson = nlohmann::json::parse(autoCompleteOptions);

	// Define all the flags for the window
	// Make it look more like a popup
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	flags |= ImGuiWindowFlags_NoFocusOnAppearing;

	// Set the position of the window
	// Also applies an offset so that the popup doesn't cover the search button or the input box
	ImGui::SetNextWindowPos(ImVec2(acOffset * 1.1f + chemicalInputPos.x, chemicalInputPos.y));

	// Create the window
	if (ImGui::Begin("AutoComplete", NULL, flags))
	{
		// Get if the user currently has the popup selected
		// so that the user can select an option and it not close before it is selected
		autoCompleteActive = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();

		// Get the number of options to display
		int numOptions = optionsJson["total"];

		AutoCompleteOptionArr.clear();
		// Loop through each one
		for (int i = 0; i < numOptions; i++)
		{
			// Read the chemical's name from the JSON object
			std::string item = optionsJson["dictionary_terms"]["compound"][i];

			// Store the option in the class so that it can be used for keyboard selection
			AutoCompleteOptionArr.push_back(item);

			// Create a selectable menu item that displays the chemical name
			if (ImGui::MenuItem(item.c_str()))
			{
				// Close the menu now that the user has selected an option
				autoCompleteActive = false;

				// If this menu item is selected
				// Set the input box to the selected item, if it is different to what is already input
				if (std::string(inpBuf) == item) continue;
				strcpy_s(inpBuf, item.c_str());

				// Queue anouther autocomplete request
				// So that the popup menu will update with new options
				CIH::queueAutoComplete();
			}
		}
		// Finish creating the window
		ImGui::End();
	}
	// Clear the JSON object
	optionsJson.clear();
}