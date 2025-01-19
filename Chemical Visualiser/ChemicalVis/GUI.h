#pragma once

#include <iostream>
#include "RenderOptions.h"
#include "ChemicalWindow.h"


class GUI
{
public:
	ImGuiIO* io;

	//the windows
	RenderOptionsWindow renderOptions;
	ChemicalFetchWindow chemicalFetch;

	void Setup(GLFWwindow* window);
	void CreateElements();
	void Draw();

	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
	ImGuiContext* context;

	void TestingWindow();
};