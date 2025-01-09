#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <string>

//this is for multi-threading
#include <future>

#include "Chemical.h"
#include "DataFetch.h"

class ChemicalFetchWindow
{
public:
	ImGuiIO* io;
	//these will likely need to be made longer
	char out[128];
	std::future<std::string> thread;

	void Display();

	//void MultiThreadFunc();
	void checkRequest();

private:
	std::string chemicalData;
	
	ImVec2 chemicalInputPos;
	bool chemicalInputIsActive;
	bool autoCompleteActive;

	char inp[128];
	char prevInp[128];
	std::string autoCompleteOptions;
	bool autoCompleteQueued;

	bool makingRequest = false;
	int selectedInputType = 0;
	float acOffset = 0;

	void DisplayInput();
	void DisplayAutoCompleteOptions();

	void AutoComplete(std::string str);
	std::string GetData(std::string chemName);

};

/*
	Should Search the stored chemical data to see if that is already saved before it is used

	for autocomplete it should do it for cached/saved
*/