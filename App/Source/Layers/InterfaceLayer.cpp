#include "InterfaceLayer.h"

#include "Core/Application.h"
#include "View2DLayer.h"
#include "View3DLayer.h"
#include "AppLayer.h"

#include "rlImGui.h"
#include "imgui.h"
#include "windowData.h"

#include "extras/IconsFontAwesome6.h"

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
	DrawSettings();

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
	if (ImGui::Begin("\xEF\x83\x88 View 2D")) // Square
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
	if (ImGui::Begin("\xef\x86\xb2 View 3D")) // Cube
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
	if (ImGui::Begin("\xef\x84\xa9 Interface")) // Info 
	{
		static std::string chemicalInp;
		bool entered = ImGui::InputText("##Chemical Input", chemicalInp.data(), chemicalInp.capacity() + 1,
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue,
			InputTextCallback, &chemicalInp);

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_MAGNIFYING_GLASS) || entered)
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

WindowData InterfaceLayer::DrawSettings()
{
	if (ImGui::Begin("\xef\x80\x93 Settings")) // Gear
	{
		View2DLayer* layer2D = Core::Application::Get().GetLayer<View2DLayer>();
		ImGui::DragFloat("Atom Size", &(layer2D->AtomSize()), 0.001f);
		ImGui::DragFloat("Hydrogen Scale", &(layer2D->HydrogenScale()), 0.001, 0.0, 1.0);
		ImGui::DragFloat("Bond Width", &(layer2D->BondWidth()), 0.001f);
		ImGui::DragFloat("Bond Seperation 2D", &(layer2D->BondSeperation()), 0.001f);
		ImGui::DragInt("World Scale", &(layer2D->WorldScale()));

		View3DLayer* layer3D = Core::Application::Get().GetLayer<View3DLayer>();
		ImGui::DragFloat("3D Pan Sensitivity", &(layer3D->Camera().PanSensitivity()), 0.001f);
		ImGui::DragFloat("Atom Size 3D", &(layer3D->AtomSize()), 0.001f);
		ImGui::DragFloat("Hydrogen Scale 3D", &(layer3D->HydrogenScale()), 0.001f);
		ImGui::DragFloat("Bond Radius 3D", &(layer3D->BondRadius()), 0.001f);
		ImGui::DragInt("Bond Detail 3D", &(layer3D->BondDetail()));
		ImGui::DragFloat("Bond Seperation 3D", &(layer3D->BondSeperation()), 0.001f);
	}
	WindowData window = getWindowData();
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


