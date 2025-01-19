#include "ChemicalWindow.h"
#include <iostream>

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

void ChemicalFetchWindow::DisplayContent()
{
	if (ImGui::InputText("##Chemical Name", inp, IM_ARRAYSIZE(inp))) 
	{
		//if there is a change in the user input make a autocomplete request
		CIH::queueAutoComplete();
	}

	// if the user pressed enter and their is an input get the data about chemical if possible 
	// TODO : allow the enter key to be changed in a settings menu
	bool entered = ImGui::IsItemDeactivatedAfterEdit() && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter) && std::string(inp).size() > 0;

	// Position of the input box
	ImVec2 chemicalInputPos = ImGui::GetCursorScreenPos();
	// is the input box in use?
	chemicalInputIsActive = ImGui::IsItemActive();

	// If the width of the window is too small put the ontop of each other
	if (ImGui::GetWindowWidth() > 150) ImGui::SameLine();

	//if name is being input do autocomplete checks
	if (selectedInputType == 0) NameInput(chemicalInputPos);


	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 1.0f); //set the width of the next widget
	if (ImGui::Combo("##Input Type", &selectedInputType, inputTypes, IM_ARRAYSIZE(inputTypes))) 
	{
		CIH::queueAutoComplete();
	}
	ImGui::PopItemWidth(); //reset widget width to default settings

	
	//ImGui::InputInt("Chemical Count", &Chemcount);
	//ImGui::Text(CIH::isRequest() ? "1" : "0");

	// get the data about the chemical when pressed
	if (ImGui::Button("Search") || entered)
	{
		//Get the data about a chemical from pubchem
		//CIH::thread.wait();
		chemicalData = CIH::GetData(inp); 
		if (CIH::ValidateData(chemicalData) == -1) 
		{
			inputError = "Invalid Input"; //state the error to the user
		}
		else
		{
			//just for testing the power of the renderer
			for (int i = 0; i < Chemcount; i++)
			{
				//create the chemical
				globalClass::chemicals.push_back(Chemical(chemicalData));
			}
		}
		chemicalData = "";
	}

	// autocomplete window position offset to not cover the button
	acOffset = ImGui::GetItemRectSize().x;

	//If there is an error to be displayed, display it to user
	if (inputError.size() > 0)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), (std::string("ERROR: ") + inputError).c_str());
	}
}

void ChemicalFetchWindow::NameInput(ImVec2& chemicalInputPos)
{
	//show the autocomplete options in a menu
	DisplayAutoCompleteOptions(chemicalInputPos);

	// if there is a request queued. make the request
	CIH::makeAutoCompleteRequest(std::string(inp));

	//get the data from the ongoing autocomplete request 
	if (CIH::checkRequestProgress() == 1)
	{
		autoCompleteOptions = CIH::getRequestData();
	}
}


void ChemicalFetchWindow::DisplayAutoCompleteOptions(ImVec2& chemicalInputPos)
{
	//don't run function if you don't have to
	// if there is nothing to output or the textbox/popup is selected
	if (autoCompleteOptions.size() <= 0 || (!chemicalInputIsActive && !autoCompleteActive)) return;
	nlohmann::json optionsJson = nlohmann::json::parse(autoCompleteOptions);

	//ImGui::SameLine();
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	flags |= ImGuiWindowFlags_NoFocusOnAppearing;

	ImGui::SetNextWindowPos(ImVec2(acOffset*1.1f + chemicalInputPos.x, chemicalInputPos.y));
	if (ImGui::Begin("AutoComplete", NULL, flags))
	{
		autoCompleteActive = ImGui::IsWindowHovered();

		int numOptions = optionsJson["total"];
		for (int i = 0; i < numOptions; i++)
		{
			//add the item to a list in a menu so that the user can select it 
			std::string item = optionsJson["dictionary_terms"]["compound"][i];
			if (ImGui::MenuItem(item.c_str()))
			{
				strcpy_s(inp, item.c_str());
				CIH::queueAutoComplete();
				autoCompleteActive = false;
			}
		}
		
		ImGui::End();
	}
	optionsJson.clear();
}
