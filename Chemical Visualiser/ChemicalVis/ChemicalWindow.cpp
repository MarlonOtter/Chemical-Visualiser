#include "ChemicalWindow.h"
#include <iostream>

void ChemicalFetchWindow::Display()
{
	//create the window
	ImGui::Begin("Chemical Fetch Editor");

	//Display t
	
	DisplayInput();

	//finish creating the window
	ImGui::End();
}

void ChemicalFetchWindow::DisplayInput()
{
	//ImGui::InputText("Test", inp, IM_ARRAYSIZE(inp));
	if (ImGui::InputText("Chemical Name", inp, IM_ARRAYSIZE(inp)))
	{
		//if there isn't currently a request and the user changes the input
		if (!makingRequest)
		{
			//give the user autocomplete options
			AutoComplete(std::string(inp));
		}
		else
		{
			autoCompleteQueued = true;
		}
	}

	if (!makingRequest && autoCompleteQueued)
	{
		AutoComplete(std::string(inp));
		autoCompleteQueued = false;
	}

	if (makingRequest)
	{
		checkRequest();
	}
	ImGui::Text(makingRequest ? "1" : "0");

	const char* inputTypes[] = { "Name", "CID", "SMILES Structure" };

	ImGui::Combo("Input Type", &selectedInputType, inputTypes, IM_ARRAYSIZE(inputTypes));
	//use a popup to list all the options that are given for auto corrrection
	ImGui::Text(autoCompleteOptions.c_str());
	
	if (ImGui::Button("Get Data"))
	{
		std::cout << "Pressed\n";
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

void ChemicalFetchWindow::MakeFinalRequest()
{

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