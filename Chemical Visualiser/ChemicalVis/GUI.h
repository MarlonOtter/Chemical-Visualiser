#pragma once

#include <iostream>
#include "RenderOptions.h"
#include "ChemicalWindow.h"


class GUI
{
public:
	static ImGuiIO* io;

	//the ImGui windows
	//static RenderOptionsWindow renderOptions;
	static ChemicalFetchWindow chemicalFetch;

	static void Setup(GLFWwindow* window);
	static void CreateElements();
	static void Draw();

	static GLFWwindow* getWindow();

private:
	static GLFWwindow* window;
	static ImGuiContext* context;
};





