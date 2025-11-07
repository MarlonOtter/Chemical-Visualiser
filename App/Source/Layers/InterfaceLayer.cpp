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
	SetStyle();
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

	if (m_ShowDemo)
	{
		ImGui::ShowDemoWindow();
	}

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

	static ImGuiID lastDockID = 0;
	static bool lastDocked = false;

	bool currentDocked = ImGui::IsWindowDocked();
	ImGuiID currentDockID = ImGui::GetWindowDockID();
	bool dockChange = false;
	if (lastDocked != currentDocked || lastDockID != currentDockID)
	{
		lastDocked = currentDocked;
		lastDockID = currentDockID;
		dockChange = true;
	}

	window = getWindowData(!open, dockChange);
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

	static ImGuiID lastDockID = 0;
	static bool lastDocked = false;

	bool currentDocked = ImGui::IsWindowDocked();
	ImGuiID currentDockID = ImGui::GetWindowDockID();
	bool dockChange = false;
	if (lastDocked != currentDocked || lastDockID != currentDockID)
	{
		lastDocked = currentDocked;
		lastDockID = currentDockID;
		dockChange = true;
	}

	WindowData window = getWindowData(!open, dockChange);
	ImGui::End();
	return window;
}

#define InputBufferSize 256
WindowData InterfaceLayer::DrawMainInterface()
{
	static WindowData window;
	bool open = ImGui::Begin("\xef\x84\xa9 Interface "); // Info i
	if (open)
	{
		static std::string chemicalInp;
		
		char buffer[InputBufferSize] = {};
		std::strncpy(buffer, chemicalInp.c_str(), InputBufferSize - 1);

		bool entered = ImGui::InputTextWithHint("##Chemical Input", "Caffeine", buffer, sizeof(buffer),
			ImGuiInputTextFlags_EnterReturnsTrue);

		if (ImGui::IsItemEdited())
		{
			chemicalInp = std::string(buffer);
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
			//chemicalInp = std::string(buffer);
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
		
		if (!m_ChemicalInfo.Cid.empty())
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Chemical Information");
			ImGui::Text((std::string("Common Name: ") + m_ChemicalInfo.CommonName).c_str());
			ImGui::Text((std::string("IUPAC Name: ") + m_ChemicalInfo.IupacName).c_str());
			ImGui::Text((std::string("Molecular Formula: ") + m_ChemicalInfo.MolecularFormula).c_str());
			ImGui::Text((std::string("Molecular Mass: ") + m_ChemicalInfo.MolecularWeight).c_str());
			ImGui::Text((std::string("CID: ") + m_ChemicalInfo.Cid).c_str());
			ImGui::Text((std::string("SMILES: ") + m_ChemicalInfo.Smiles).c_str());
			ImGui::Text((std::string("InChI: ") + m_ChemicalInfo.InChI).c_str());
		}
	}
	window = getWindowData(!open);
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawSettings()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	Settings& settings = Core::Application::Get().GetLayer<AppLayer>()->GetSettings();
	if (settings.HasChanged())
	{
		window_flags |= ImGuiWindowFlags_UnsavedDocument;
	}

	bool open = ImGui::Begin("\xef\x80\x93 Settings ", NULL, window_flags); // Gear
	if (open)
	{
		
		auto& values = settings.Values();

		if (!settings.HasChanged())
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Apply"))
		{
			std::cout << "Saving Settings To Disk\n";
			settings.Save();
		}
		if (!settings.HasChanged())
		{
			ImGui::EndDisabled();
		}

		ImGui::SeparatorText("\xef\x83\x89 General"); // Bars

		ImGuiIO& io = ImGui::GetIO();
		// Use Bitwise OR so that all the UI is still displayed but I can detect if an entry is changed easily
		if (
			ImGui::SliderFloat("Font Size ##Global", &values.FontSize, 0.25f, 2.0f) |
			ImGui::Checkbox("Dark Mode ##GlobalUI", &values.DarkMode)
			) settings.MakeChange();

		io.FontGlobalScale = values.FontSize;
		if (values.DarkMode)
		{
			ImGui::StyleColorsDark();
			SetStyle();
		}
		else {
			ImGui::StyleColorsLight();
		}

		ImGui::SeparatorText("\xEF\x83\x88 2D Visualiser"); // Square

		if (
		ImGui::SliderFloat("Atom Size ##2D", &values.AtomScale2D, 0.01f, 2.0f) |
		ImGui::SliderFloat("Hydrogen Scale ##2D", &values.HydrogenScale2D, 0.01f, 1.0f) |
		ImGui::SliderFloat("Bond Width ##2D", &values.BondWidth2D, 0.01f, 2.0f) |
		ImGui::SliderFloat("Bond Seperation ##2D", &values.BondSeperation2D, 0.01f, 2.0f) |
		ImGui::DragInt("World Scale ##2D", &values.WorldScale2D) |
		ImGui::Checkbox("Show Element Symbol ##2D", &values.ShowElementLabels) |
		ImGui::SliderFloat("Label Scale ##2D", &values.LabelScale, 0.01f, 0.5f)
			) settings.MakeChange();


		ImGui::SeparatorText("\xef\x86\xb2 3D Visualiser"); // Cube

		if (
		ImGui::SliderFloat("Atom Size ##3D", &(values.AtomScale3D), 0.01f, 2.0f) |
		ImGui::SliderFloat("Hydrogen Scale ##3D", &(values.HydrogenScale3D), 0.01, 1.0) |
		ImGui::SliderFloat("Bond Radius ##3D", &(values.BondRadius3D), 0.01f, 2.0f) |
		ImGui::SliderFloat("Bond Detail ##3D", &(values.BondDetail3D), 0.0f, 2.0f) |
		ImGui::SliderFloat("Bond Seperation ##3D", &(values.BondSeperation3D), 0.01f, 2.0f) |
		ImGui::SliderFloat("Look Sensitivity ##3D", &(values.LookSensitivity3D), 0.0f, 3.0f) |
		ImGui::SliderFloat("Pan Sensitivity ##3D", &(values.PanSensitivity3D), 0.01f, 2.0f)
			) settings.MakeChange();


		ImGui::SeparatorText("\xef\x80\x93 Other"); // Gear

		ImGui::Checkbox("Show Demo", &m_ShowDemo);
		if (ImGui::Button("\xef\x87\xb8 Clear Cached Chemicals")) // Trash
		{
			Core::Application::Get().GetLayer<AppLayer>()->QueueDeleteCachedChemicals();
		}
	}
	
	WindowData window = getWindowData(!open);
	ImGui::End();
	return window;
}

void InterfaceLayer::OnEvent(Core::Event& event)
{
}

WindowData InterfaceLayer::getWindowData(bool closed, bool dockChange)
{
	return WindowData{
		(int)ImGui::GetContentRegionMax().x,
		(int)ImGui::GetContentRegionMax().y,
		ImGui::IsWindowHovered(),
		ImGui::IsWindowFocused(),
		false,
		closed,
		dockChange,
	};
}

void InterfaceLayer::PushError(std::string error)
{
	std::cout << "ERROR: " << error << "\n";
	return;
}

void InterfaceLayer::SetStyle()
{
	// Set style values (font DPI scaling and rounding)
	ImGuiStyle& style = ImGui::GetStyle();
	style.FontScaleDpi = std::min(GetWindowScaleDPI().x, GetWindowScaleDPI().y);
	style.FrameRounding = 8;
	style.WindowRounding = 8;
	style.GrabRounding = 8;
	style.TabRounding = 8;

	// Set Style Colours
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.72f, 0.72f, 0.72f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.65f, 0.65f, 0.65f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.60f, 0.60f, 0.60f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.71f, 0.72f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.51f, 0.51f, 0.51f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.91f, 0.91f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.48f, 0.48f, 0.95f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.80f);
	colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.22f, 0.22f, 0.86f);
	colors[ImGuiCol_TabSelected] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
	colors[ImGuiCol_TabDimmed] = ImVec4(0.12f, 0.12f, 0.12f, 0.97f);
	colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.87f, 0.87f, 0.87f, 0.70f);
	colors[ImGuiCol_NavCursor] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
}

