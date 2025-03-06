#include "GUI.h"

ImGuiIO* GUI::io = nullptr;

//the ImGui windows
//RenderOptionsWindow GUI::renderOptions;
ChemicalFetchWindow GUI::chemicalFetch;

GLFWwindow* GUI::window = nullptr;
ImGuiContext* GUI::context = nullptr;


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
	//renderOptions.Display();
	chemicalFetch.Display();
	
	ImGui::ShowDemoWindow();
}

void GUI::Draw()
{
	// Render the GUI
	ImGui::Render();

	// Using the Draw Data
	// Draw the UI to the screen using OpenGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* GUI::getWindow()
{
	return window;
}