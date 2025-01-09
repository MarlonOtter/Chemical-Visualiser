#include "GUI.h"

chemVisGUI::chemVisGUI(GLFWwindow* window)
{
	chemVisGUI::window = window;
	Setup();
}

void chemVisGUI::CreateElements()
{
	//set context
	ImGui::SetCurrentContext(cont);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	//display windows
	renderOptions.Display();
	chemicalFetch.Display();
	
	ImGui::ShowDemoWindow();
}

void chemVisGUI::TestingWindow()
{
	
}

void chemVisGUI::Draw()
{
	//render the GUI
	ImGui::Render();
	//for the backend (very much needed)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int chemVisGUI::Setup()
{
	//Initialise ImGUI
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	
	chemVisGUI::io = &ImGui::GetIO();
	cont = ImGui::GetCurrentContext();
	return 0;
}