#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <string>

//this is for multi-threading
#include <future>

#include "DataFetch.h"

class ChemicalFetchWindow
{
public:
	//these will likely need to be made longer
	char out[128];
	std::future<std::string> thread;

	void Display();

	//void MultiThreadFunc();
	void checkRequest();

private:
	char inp[128];
	char prevInp[128];
	std::string autoCompleteOptions;
	bool autoCompleteQueued;

	bool makingRequest = false;
	int selectedInputType = 0;

	void DisplayInput();
	void AutoComplete(std::string str);
	void MakeFinalRequest();

};