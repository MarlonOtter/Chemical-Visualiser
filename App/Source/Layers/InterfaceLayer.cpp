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
	
	ImGui::GetStyle().FontScaleDpi = std::min(GetWindowScaleDPI().x, GetWindowScaleDPI().y);
}

InterfaceLayer::~InterfaceLayer()
{
	rlImGuiShutdown();
}

void InterfaceLayer::Update(float ts)
{
	m_TimeSinceLastInput += ts;
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
	//window_flags |= ImGuiWindowFlags_MenuBar;
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
	bool open = (ImGui::Begin("\xEF\x83\x88 View 2D ", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)); // Square
	if (open)
	{
		if (renderTexture2D.has_value())
		{
			ImGui::Image((void*)(intptr_t)renderTexture2D->get().texture.id, { static_cast<float>(renderTexture2D->get().texture.width), static_cast<float>(renderTexture2D->get().texture.height) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	}
	window = getWindowData(!open);
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawView3D()
{
	bool open = ImGui::Begin("\xef\x86\xb2 View 3D ", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); // Cube;
	if (open)
	{
		if (renderTexture3D.has_value())
		{
			ImGui::Image((void*)(intptr_t)renderTexture3D->get().texture.id, { static_cast<float>(renderTexture3D->get().texture.width), static_cast<float>(renderTexture3D->get().texture.height) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	}
	WindowData window = getWindowData(!open);
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawMainInterface()
{
	static WindowData window;
	bool open = ImGui::Begin("\xef\x84\xa9 Interface "); // Info i
	if (open)
	{
		static std::string chemicalInp;
		bool entered = ImGui::InputTextWithHint("##Chemical Input", "Caffeine", chemicalInp.data(), chemicalInp.capacity() + 1,
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue,
			InputTextCallback, &chemicalInp);

		if (ImGui::IsItemEdited())
		{
			m_TimeSinceLastInput = 0.0f;
			m_MadeRequest = false;
		}

		if (ImGui::IsItemActive()) {
			// Wait for 0.25 seconds of no user input before making a request
			if (m_TimeSinceLastInput >= 0.25f && !m_MadeRequest)
			{
				Core::Application::Get().GetLayer<AppLayer>()->RequestNewAutoComplete(chemicalInp);
				m_MadeRequest = true;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_MAGNIFYING_GLASS) || entered)
		{
			entered = false;
			// send to app layer to fetch
			Core::Application::Get().GetLayer<AppLayer>()->SetChemical(chemicalInp);
		}

		if (m_AutoCompleteOptions.size() > 0)
		{
			static int selected = 0;

			std::vector<const char*> items;
			for (const auto& option : m_AutoCompleteOptions) {
				items.push_back(option.c_str());
			}

			bool changedOption = ImGui::ListBox("##Chemical Autocomplete", &selected, items.data(), static_cast<int>(items.size()));
			if (changedOption)
			{
				chemicalInp = m_AutoCompleteOptions[selected];
			}
		}

	}
	window = getWindowData(!open);
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawSettings()
{
	
	bool open = ImGui::Begin("\xef\x80\x93 Settings "); // Gear
	if (open)
	{

		ImGui::SeparatorText("\xef\x83\x89 General"); // Bars

		ImGuiIO& io = ImGui::GetIO();
		ImGui::SliderFloat("Font Size ##Global", &io.FontGlobalScale, 0.25f, 4.0f);
		//ImGui::SliderFloat("Window Rounding"); 
		//ImGui::SliderFloat("Widget Rounding");
		
		static bool StyleDark = true;
		if (ImGui::Checkbox("Dark Mode ##GlobalUI", &StyleDark))
		{
			if (StyleDark)
			{
				ImGui::StyleColorsDark();
			}
			else {
				ImGui::StyleColorsLight();
			}
		}


		ImGui::SeparatorText("\xEF\x83\x88 2D Visualiser"); // Square

		View2DLayer* layer2D = Core::Application::Get().GetLayer<View2DLayer>();
		ImGui::SliderFloat("Atom Size ##2D", &(layer2D->AtomSize()), 0.01f, 0.75f);
		ImGui::SliderFloat("Hydrogen Scale ##2D", &(layer2D->HydrogenScale()), 0.01f, 1.0f);
		ImGui::SliderFloat("Bond Width ##2D", &(layer2D->BondWidth()), 0.01f, 0.5f);
		ImGui::SliderFloat("Bond Seperation ##2D", &(layer2D->BondSeperation()), 0.01f, 0.75f);
		ImGui::DragInt("World Scale ##2D", &(layer2D->WorldScale()));

		ImGui::SeparatorText("\xef\x86\xb2 3D Visualiser"); // Cube

		View3DLayer* layer3D = Core::Application::Get().GetLayer<View3DLayer>();
		ImGui::DragFloat("Pan Sensitivity ##3D", &(layer3D->Camera().PanSensitivity()), 0.001f);
		ImGui::SliderFloat("Atom Size ##3D", &(layer3D->AtomSize()), 0.01f, 0.75f);
		ImGui::SliderFloat("Hydrogen Scale ##3D", &(layer3D->HydrogenScale()), 0.01, 1.0);
		ImGui::SliderFloat("Bond Radius ##3D", &(layer3D->BondRadius()), 0.001f, 0.25f);
		ImGui::SliderInt("Bond Detail ##3D", &(layer3D->BondDetail()), 10, 50);
		ImGui::SliderFloat("Bond Seperation ##3D", &(layer3D->BondSeperation()), 0.01f, 0.75f);
	}
	WindowData window = getWindowData(!open);
	ImGui::End();
	return window;
}

void InterfaceLayer::OnEvent(Core::Event& event)
{
}

WindowData InterfaceLayer::getWindowData(bool closed)
{
	return WindowData{
		(int)ImGui::GetContentRegionMax().x,
		(int)ImGui::GetContentRegionMax().y,
		ImGui::IsWindowHovered(),
		ImGui::IsWindowFocused(),
		closed,
	};
}

void InterfaceLayer::PushError(std::string error)
{
	std::cout << "ERROR: " << error << "\n";
	return;
}


