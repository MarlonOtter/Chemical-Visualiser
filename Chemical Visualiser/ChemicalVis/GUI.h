#pragma once

#include <iostream>
#include "RenderOptions.h"
#include "ChemicalWindow.h"
#include "Settings.h"
#include "../SettingsUI.h"

class GUI
{
public:
	static ImGuiIO* io;

	//the ImGui windows
	//static RenderOptionsWindow renderOptions;
	static ChemicalFetchWindow chemicalFetch;

	static void Setup(GLFWwindow* window);
	static void CreateElements(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed);
	static void Draw();

	static GLFWwindow* getWindow();

	static void TempararyGUI(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed);

private:
	static GLFWwindow* window;
	static ImGuiContext* context;
};





