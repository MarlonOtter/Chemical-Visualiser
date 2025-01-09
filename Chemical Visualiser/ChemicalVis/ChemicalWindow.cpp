#include "ChemicalWindow.h"
#include <iostream>

void ChemicalFetchWindow::Display()
{
	//create the window
	ImGui::Begin("Chemical Fetch Window", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus);
	io = &ImGui::GetIO();

	//Display the input
	DisplayInput();
	

	//finish creating the window
	ImGui::End();
}

void ChemicalFetchWindow::DisplayInput()
{
	//ImGui::InputText("Test", inp, IM_ARRAYSIZE(inp));
	//if (ImGui::IsPopupOpen("AutoComplete")) ImGui::SetKeyboardFocusHere();
	if (ImGui::InputText("##Chemical Name", inp, IM_ARRAYSIZE(inp))) {
		//queue a request
		autoCompleteQueued = true;
	}
	chemicalInputPos = ImGui::GetCursorScreenPos();
	chemicalInputIsActive = ImGui::IsItemActive();
	ImGui::SameLine();

	//if the name is not being input
	// dont need to check using autocomplete
	if (selectedInputType != 0) autoCompleteQueued = false;

	// if there is a request queued. make the request
	if (!makingRequest && autoCompleteQueued) {
		AutoComplete(std::string(inp));
		autoCompleteQueued = false;
	}
	//if there is a request in progress check if it is complete
	if (makingRequest) checkRequest();


	//show the autocomplete options in a menu
	if (selectedInputType == 0) DisplayAutoCompleteOptions();
	
	// different input types
	const char* inputTypes[] = { "Name", "CID", "SMILES" };
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.9f);
	if (ImGui::Combo("##Input Type", &selectedInputType, inputTypes, IM_ARRAYSIZE(inputTypes))) autoCompleteQueued = true;
	ImGui::PopItemWidth();

	// get the data about the chemical
	if (ImGui::Button("Search")) chemicalData = GetData(inp);
	acOffset = ImGui::GetItemRectSize().x;
}

void ChemicalFetchWindow::DisplayAutoCompleteOptions()
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
				autoCompleteQueued = true;
				autoCompleteActive = false;
			}
		}
		
		ImGui::End();
	}
}

void ChemicalFetchWindow::AutoComplete(std::string str)
{

		//this should be done on anouther thread to reduce the performance impact
		try {
			//create thread
			thread = std::async(std::launch::async, PubChem::autoComplete, str, 5);
			makingRequest = true;
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in thread: " << e.what() << std::endl;
		}
}

std::string ChemicalFetchWindow::GetData(std::string chemName)
{
	return PubChem::name(chemName);
}

void ChemicalFetchWindow::checkRequest()
{
	std::future_status status = thread.wait_for(std::chrono::seconds(0));
	if (status == std::future_status::ready)
	{
		autoCompleteOptions = thread.get();
		makingRequest = false;
	}
}