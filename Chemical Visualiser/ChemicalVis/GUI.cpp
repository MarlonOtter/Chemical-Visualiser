#include "GUI.h"

void GUI::Setup(GLFWwindow* window)
{
	GUI::window = window;
	
	//Initialise ImGUI
	ImGui::CreateContext();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(GUI::window, true);

	GUI::io = &ImGui::GetIO();

	context = ImGui::GetCurrentContext();
}

void GUI::CreateElements()
{
	//set context
	ImGui::SetCurrentContext(context);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	//display windows
	renderOptions.Display();
	chemicalFetch.Display();
	
	ImGui::ShowDemoWindow();
}

void GUI::TestingWindow()
{
	
}

void GUI::Draw()
{
	//render the GUI
	ImGui::Render();
	//for the backend (very much needed)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* GUI::getWindow()
{
	return window;
}