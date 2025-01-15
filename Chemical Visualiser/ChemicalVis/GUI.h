#pragma once

#include <iostream>
#include "RenderOptions.h"
#include "ChemicalWindow.h"


class chemVisGUI
{
public:
	ImGuiIO* io;

	//the windows
	RenderOptionsWindow renderOptions;
	ChemicalFetchWindow chemicalFetch;

	chemVisGUI(GLFWwindow* window);
	void CreateElements();
	void Draw();

private:
	GLFWwindow* window;
	ImGuiContext* cont;

	int Setup();

	void TestingWindow();
};