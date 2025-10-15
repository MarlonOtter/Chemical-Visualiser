#include "InterfaceLayer.h"

#include "Core/Application.h"
#include "View2DLayer.h"
#include "View3DLayer.h"

#include "rlImGui.h"
#include "imgui.h"
#include "windowData.h"

InterfaceLayer::InterfaceLayer()
{
	rlImGuiSetup(true);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

InterfaceLayer::~InterfaceLayer()
{
	rlImGuiShutdown();
}

void InterfaceLayer::Update(float ts)
{

}

void InterfaceLayer::OnComposite()
{
	rlImGuiBegin();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpaceHost", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();

	/*-------------------------------*/

	ImGui::Begin("View 2D");

	if (renderTexture2D.has_value())
	{
		ImGui::Image((void*)(intptr_t)renderTexture2D->get().texture.id, ImGui::GetContentRegionAvail(), ImVec2{0, 1}, ImVec2{1, 0});
	}
	
	window2D = getWindowData();
	//std::cout << "width: " << window2D.width << ", height: " << window2D.height << "\n";

	ImGui::End();

	/*-------------------------------*/

	ImGui::Begin("View 3D");
	
	if (renderTexture3D.has_value())
	{
		ImGui::Image((void*)(intptr_t)renderTexture3D->get().texture.id, ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	window3D = getWindowData();

	ImGui::End();

	/*-------------------------------*/

	ImGui::Begin("Interface");
	ImGui::Text("This is the main interface");
	ImGui::End();

	rlImGuiEnd();

	Core::Application& app = Core::Application::Get();
	app.GetLayer<View2DLayer>()->setWindowData(window2D);
	app.GetLayer<View3DLayer>()->setWindowData(window3D);
}

void InterfaceLayer::OnEvent(Core::Event& event)
{

}

WindowData InterfaceLayer::getWindowData()
{
	return WindowData{
		(int)ImGui::GetWindowWidth(),
		(int)ImGui::GetWindowHeight(),
		ImGui::IsWindowHovered(),
		ImGui::IsWindowFocused(),
	};
}