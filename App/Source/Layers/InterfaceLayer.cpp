#include "InterfaceLayer.h"

#include "Core/Application.h"
#include "View2DLayer.h"
#include "View3DLayer.h"
#include "AppLayer.h"

#include "rlImGui.h"
#include "imgui.h"
#include "windowData.h"


static int InputTextCallback(ImGuiInputTextCallbackData* data) {
	if (data->UserData) {
		std::string* str = static_cast<std::string*>(data->UserData);
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
			str->resize(data->BufTextLen);
			data->Buf = str->data();
		}
	}
	return 0;
}


InterfaceLayer::InterfaceLayer()
{
	rlImGuiSetup(true);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
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

	DrawDockSpace();
	window2D = DrawView2D();
	window3D = DrawView3D();
	DrawMainInterface();

	ImGui::ShowDemoWindow();

	rlImGuiEnd();

	Core::Application& app = Core::Application::Get();
	app.GetLayer<View2DLayer>()->setWindowData(window2D);
	app.GetLayer<View3DLayer>()->setWindowData(window3D);
}

void InterfaceLayer::DrawDockSpace()
{
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

	if (ImGui::Begin("DockSpaceHost", nullptr, window_flags))
	{
		ImGui::PopStyleVar(3);
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	ImGui::End();
}

WindowData InterfaceLayer::DrawView2D()
{
	static WindowData window;
	if (ImGui::Begin("View 2D"))
	{
		if (renderTexture2D.has_value())
		{
			ImGui::Image((void*)(intptr_t)renderTexture2D->get().texture.id, ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		window = getWindowData();
	}
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawView3D()
{
	static WindowData window;
	if (ImGui::Begin("View 3D"))
	{
		if (renderTexture3D.has_value())
		{
			ImGui::Image((void*)(intptr_t)renderTexture3D->get().texture.id, ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		window = getWindowData();
	}
	ImGui::End();
	
	return window;
}

WindowData InterfaceLayer::DrawMainInterface()
{
	static WindowData window;
	if (ImGui::Begin("Interface"))
	{
		static std::string chemicalInp;
		ImGui::InputText("##Chemical Input", chemicalInp.data(), chemicalInp.capacity() + 1,
			ImGuiInputTextFlags_CallbackResize,
			InputTextCallback, &chemicalInp);

		static bool entered = false;
		if (ImGui::Button("Search") || entered)
		{
			entered = false;
			// send to app layer to fetch
			Core::Application::Get().GetLayer<AppLayer>()->SetChemical(chemicalInp);
		}

		ImGui::Text("Here will also be the information about the chemical");
		window = getWindowData();
	}
	ImGui::End();
	return window;
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
		true,
	};
}

void InterfaceLayer::PushError(std::string error)
{
	return;
}


