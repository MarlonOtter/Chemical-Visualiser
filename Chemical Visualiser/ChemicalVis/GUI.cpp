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
	//ImGui::CreateContext();
	context = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(context);

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(GUI::window, true);

	GUI::io = &ImGui::GetIO();
	//GUI::io->Fonts->AddFontDefault();
	ImFont* font = GUI::io->Fonts->AddFontFromFileTTF("Resources/Fonts/FreeSans.ttf", 32.0f); // I should make this changeable in settings
	GUI::io->Fonts->Build();
	ImGui_ImplOpenGL3_CreateFontsTexture();

	io->FontDefault = font;
}

void GUI::CreateElements(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed)
{
	if (context == nullptr)
	{
		return;
	}
	//set context
	ImGui::SetCurrentContext(context);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	//display windows
	//renderOptions.Display();
	chemicalFetch.Display();
	
	ImGui::ShowDemoWindow();

	GUI::TempararyGUI(settingInp, num, settingsUI, cameraSpeed);
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

void GUI::TempararyGUI(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed)
{
	if (ImGui::Begin("Settings Class Testing"))
	{

		if (ImGui::Button("Read File"))
		{
			Settings::Refresh();
		}

		if (ImGui::Button("Save to File"))
		{
			Settings::Save();
		}

		ImGui::InputText("Setting", settingInp, 128);

		if (ImGui::Button("Find"))
		{
			double startTime = glfwGetTime();
			Setting setting = Settings::GetAll(std::string(settingInp));

			std::cout << "Duration: " << (glfwGetTime() - startTime) * 1000.0 << "ms\n";
			std::cout << "Type: " << setting.type << "\nName: " << setting.name << "\n";
		}
		ImGui::SliderFloat("Number", num, 0.001f, 100.0f);
		if (ImGui::Button("Set"))
		{
			std::cout << Settings::Set(std::string(settingInp), *num) << "\n";
		}

		if (ImGui::Button("List Children"))
		{
			auto vec = Settings::getChildren("/" + std::string(settingInp));
			std::cout << "Found " << vec.size() << " Children Objects\n";
		}
		ImGui::Checkbox("Settings Changed", &Settings::changed);

	}
	ImGui::End();

	settingsUI.Draw();



	//create a temparary UI to tweak atom size and clear of all atoms
	ImGui::Begin("Atom Settings");
	ImGui::DragFloat("atom scale", &globalClass::atomScale, 0.001f, 0.001f, 100.0f);
	ImGui::DragFloat("bond width", &globalClass::bondRadius, 0.001f, 0.001f, 1000.0f);
	ImGui::DragFloat("bond length", &globalClass::bondLengthMultiplier, 0.001f, 0.001f, 1000.0f);
	ImGui::DragFloat("bond seperation Dist", &globalClass::bondSeperationDist, 0.001f, 0.001f, 10.0f);
	ImGui::DragFloat("chemical seperation Dist", &globalClass::chemicalSeperationDist, 0.001f, 0.001f, 10.0f);

	//trying to get a value that can alter the speed of rotation
	ImGui::DragFloat("cameraSpeed", &cameraSpeed, 0.01f, 0.0001f, 10.0f);

	if (ImGui::Button("Clear Chemicals"))
	{
		for (int i = 0; i < globalClass::chemicals.size(); i++)
		{
			globalClass::chemicals[i].atoms.clear();
		}
		globalClass::chemicals.clear();
	}
	ImGui::End();



}
