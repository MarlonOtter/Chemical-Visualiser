#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <string>

//this is for multi-threading


#include "globalClass.h"
#include "PubchemAccess.h"
#include "ChemicalInputHandling.h"

static int InputTextCallback(ImGuiInputTextCallbackData* data);

class ChemicalFetchWindow
{
public:
	ImGuiIO* io = nullptr;

	bool gotChemical = false;

	void Display();

	void setBuff(char* buf, int size);
private:
	// is true only on the first frame
	bool init = true;

	//the chemical data as a string
	chemicalData chemicalData;
	
	int Chemcount = 1;

	// is the chemical input or popup selected
	bool chemicalInputIsActive = false;
	bool autoCompleteActive = false;
	bool inputFocus = false;

	// string that displays error to user
	std::string inputError = "";

	// stores the input text
	char inpBuf[128] = "";
	std::string inpValue = "";

	bool queueTab = false;

	// autocomplete Stuff
	std::string autoCompleteOptions = "";
	std::vector<std::string> AutoCompleteOptionArr = { };
	int selectedAutoCompleteOption = -1;

	// combo for selecting input type
	const char* inputTypes[3] = { "Name", "CID", "SMILES" };
	int selectedInputType = 0;

	//offset for the autocomplete popup
	float acOffset = 0;

	//Display main UI
	void DisplayContent();
	//Display Autocomplete Options
	void DisplayAutoCompleteOptions(ImVec2& chemicalInputPos);

	// Ran when user selects name as method of input
	void NameInput(ImVec2& chemicalInputPos);	
	void CIDInput();
	void SMILESInput();
};