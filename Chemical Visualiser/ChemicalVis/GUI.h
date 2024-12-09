#pragma once

#include <iostream>
#include "EditorWindow.h"
#include "RenderOptions.h"
#include "ChemicalWindow.h"

/*
Notes:
	Inorder to interact with the visualiser a mouse is needed
	to solve this a window can be set to act as the program so
	that when that is hovered over/selected it will use the
	actual visualiser
*/


class chemVisGUI
{
public:
	ImGuiIO* io;

	//all the windows
	EditorWindow editor;
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