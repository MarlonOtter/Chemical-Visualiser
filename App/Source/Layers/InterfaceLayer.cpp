#include "InterfaceLayer.h"

#include "Core/Application.h"
#include "View2DLayer.h"
#include "View3DLayer.h"
#include "AppLayer.h"

#include "ChemVis/Exporter.h"

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
	SetDarkStyle();
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

	DrawMenuBar();

	DrawDockSpace();
	window2D = DrawView2D();
	window3D = DrawView3D();
	DrawMainInterface();
	
	if (m_ShowSettings) DrawSettings();
	if (m_ShowExport) DrawExport();

	if (m_ShowDemo) ImGui::ShowDemoWindow(&m_ShowDemo);
	

	rlImGuiEnd();

	Core::Application& app = Core::Application::Get();
	app.GetLayer<View2DLayer>()->setWindowData(window2D);
	app.GetLayer<View3DLayer>()->setWindowData(window3D);
}

void InterfaceLayer::DrawDockSpace()
{
	ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->WorkPos);
	ImGui::SetNextWindowSize(Viewport->WorkSize);
	ImGui::SetNextWindowViewport(Viewport->ID);

	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("DockSpaceHost", nullptr, WindowFlags))
	{
		ImGui::PopStyleVar(3);
		ImGuiID DockSpaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(DockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	ImGui::End();
}

void InterfaceLayer::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Export", nullptr, &m_ShowExport);
			ImGui::MenuItem("Import"); // TODO : Implement Import Functionality
			
			if (ImGui::BeginMenu("Cache"))
			{
				// TODO : Display List of all cached chemicals
				ImGui::MenuItem("View");
				
				bool CacheEmpty = Core::Application::Get().GetLayer<AppLayer>()->IsCacheEmpty();
				if (CacheEmpty) ImGui::BeginDisabled();
				if (ImGui::MenuItem("Clear"))
				{
					// TODO : Confirm clear then send message to confirm that the cache has been cleared
					Core::Application::Get().GetLayer<AppLayer>()->QueueDeleteCachedChemicals();
				}
				if (CacheEmpty) ImGui::EndDisabled();

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Settings", nullptr, &m_ShowSettings);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Documentation"); // TODO : Link to online docs
			ImGui::MenuItem("Send Feedback"); // TODO : Link to feedback form
			ImGui::MenuItem("Dear ImGui Demo", nullptr, &m_ShowDemo);
			ImGui::MenuItem("About"); // TODO : Opens an About Window
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
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

	window = GetWindowData(!open, dockChange);
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

	WindowData window = GetWindowData(!open, dockChange);
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

		bool entered = ImGui::InputTextWithHint("##Chemical Input", "Aspirin", buffer, sizeof(buffer),
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
	window = GetWindowData(!open);
	ImGui::End();
	return window;
}


WindowData InterfaceLayer::DrawSettings()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
	Settings& settings = Core::Application::Get().GetLayer<AppLayer>()->GetSettings();
	if (settings.HasChanged())
	{
		window_flags |= ImGuiWindowFlags_UnsavedDocument;
	}

	bool open = ImGui::Begin("\xef\x80\x93 Settings ", &m_ShowSettings, window_flags); // Gear
	if (open)
	{
		auto& values = settings.Values();

		bool SettingsChanged = settings.HasChanged();
		if (!SettingsChanged) ImGui::BeginDisabled();
		
		if (ImGui::Button("Save"))
		{
			std::cout << "Saving Settings To Disk\n";
			settings.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Undo")) settings.QueueRevert();
		
		if (!SettingsChanged) ImGui::EndDisabled();

		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			std::cout << "Resetting Settings To Default\n";
			settings.Reset();
		}

		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::BeginTabBar("SettingsTabs"))
		{
			if (ImGui::BeginTabItem("\xef\x83\x89 General ##SettingTab")) // Bars
			{
				// Use Bitwise OR so that all the UI is still displayed but I can detect if an entry is changed easily
				if (
					ImGui::SliderFloat("Font Size ##Global", &values.FontSize, 0.25f, 2.0f) |
					ImGui::Checkbox("Dark Mode ##GlobalUI", &values.DarkMode) |
					ImGui::SliderInt("Target Framerate ##Global", &values.TargetFPS, 15, 240) |
					ImGui::Checkbox("Dynamic Framerate ##Global", &values.DynamicFramerate)
					) settings.MakeChange();
				

				// If dark mode setting has been reverted, it will also need to update any changes
				if (m_PreviousDarkMode != values.DarkMode)
				{
					if (values.DarkMode)
					{
						ImGui::StyleColorsDark();
						SetDarkStyle();
					}
					else {
						ImGui::StyleColorsLight();
					}
					m_PreviousDarkMode = values.DarkMode;
				}
				io.FontGlobalScale = values.FontSize;
				
				

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("\xEF\x83\x88 2D ##SettingTab")) // Square
			{
				float backgroundColor[3] = {
					static_cast<float>(values.BackgroundColor2D[0]) / 255.0f,
					static_cast<float>(values.BackgroundColor2D[1]) / 255.0f,
					static_cast<float>(values.BackgroundColor2D[2]) / 255.0f
				};

				if (
					ImGui::Checkbox("Background ##2D", &values.Background2D) |
					// Only Draw the background colour edit if background is enabled
					(values.Background2D && ImGui::ColorEdit3("Background Color ##2D", backgroundColor, ImGuiColorEditFlags_DisplayHex)) |
					ImGui::SliderFloat("Atom Size ##2D", &values.AtomScale2D, 0.01f, 2.0f) |
					ImGui::SliderFloat("Hydrogen Scale ##2D", &values.HydrogenScale2D, 0.01f, 1.0f) |
					ImGui::SliderFloat("Bond Width ##2D", &values.BondWidth2D, 0.01f, 2.0f) |
					ImGui::SliderFloat("Bond Seperation ##2D", &values.BondSeperation2D, 0.01f, 2.0f) |
					ImGui::DragInt("World Scale ##2D", &values.WorldScale2D) |
					ImGui::Checkbox("Show Element Symbol ##2D", &values.ShowElementLabels) |
					ImGui::SliderFloat("Label Scale ##2D", &values.LabelScale, 0.01f, 0.5f) |
					ImGui::SliderFloat("Camera Smoothing ##2D", &values.CameraSmoothing2D, 0.0f, 1.0f)
					)
				{
					settings.MakeChange();
					values.BackgroundColor2D = {
						static_cast<uint8_t>(std::roundf(backgroundColor[0] * 255.0f)),
						static_cast<uint8_t>(std::roundf(backgroundColor[1] * 255.0f)),
						static_cast<uint8_t>(std::roundf(backgroundColor[2] * 255.0f))
					};
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("\xef\x86\xb2 3D ##SettingTab")) // Cube
			{
				float backgroundColor[3] = { 
					static_cast<float>(values.BackgroundColor3D[0]) / 255.0f,
					static_cast<float>(values.BackgroundColor3D[1]) / 255.0f,
					static_cast<float>(values.BackgroundColor3D[2]) / 255.0f
				};

				if (
					ImGui::Checkbox("Background ##3D", &values.Background3D) |
					// Only Draw the background colour edit if background is enabled
					(values.Background3D && ImGui::ColorEdit3("Background Color ##3D", backgroundColor, ImGuiColorEditFlags_DisplayHex)) |
					ImGui::SliderFloat("Atom Size ##3D", &(values.AtomScale3D), 0.01f, 2.0f) |
					ImGui::SliderFloat("Hydrogen Scale ##3D", &(values.HydrogenScale3D), 0.01, 1.0) |
					ImGui::SliderFloat("Bond Radius ##3D", &(values.BondRadius3D), 0.01f, 2.0f) |
					ImGui::SliderFloat("Bond Detail ##3D", &(values.BondDetail3D), 0.0f, 2.0f) |
					ImGui::SliderFloat("Bond Seperation ##3D", &(values.BondSeperation3D), 0.01f, 2.0f) |
					ImGui::SliderFloat("Look Sensitivity ##3D", &(values.LookSensitivity3D), 0.0f, 3.0f) |
					ImGui::SliderFloat("Pan Sensitivity ##3D", &(values.PanSensitivity3D), 0.01f, 2.0f) |
					ImGui::SliderFloat("Camera Smoothing ##3D", &(values.CameraSmoothing3D), 0.0f, 1.0f)
					) 
				{
					settings.MakeChange();
					values.BackgroundColor3D = {
						static_cast<uint8_t>(std::roundf(backgroundColor[0] * 255.0f)),
						static_cast<uint8_t>(std::roundf(backgroundColor[1] * 255.0f)),
						static_cast<uint8_t>(std::roundf(backgroundColor[2] * 255.0f))
					};
				}
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Element Colours ##SettingTab"))
			{
				for (size_t i = 0; i < 118; i++)
				{
					size_t index = i * 3;
					float colour[3] = {
						Core::Uint8ToFloat(values.ElementColors[index]),
						Core::Uint8ToFloat(values.ElementColors[index + 1]),
						Core::Uint8ToFloat(values.ElementColors[index + 2]),
					};

					std::string label = ChemVis::Chemical::GetAtomSymbol(i+1);
					if (ImGui::ColorEdit3(label.c_str(), colour))
					{
						settings.MakeChange();
						values.ElementColors[index] = Core::FloatToUint8(colour[0]);
						values.ElementColors[index + 1] = Core::FloatToUint8(colour[1]);
						values.ElementColors[index + 2] = Core::FloatToUint8(colour[2]);
					}

				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	WindowData window = GetWindowData(!open);
	ImGui::End();
	return window;
}

WindowData InterfaceLayer::DrawExport()
{
	if (ImGui::Begin("Export", &m_ShowExport));
	{
		static int SelectedView = 0; 
		const char* Views[] = { "2D", "3D" };
		ImGui::Combo("View", &SelectedView, Views, IM_ARRAYSIZE(Views));

		if (ImGui::BeginTabBar("ExportType"))
		{
			if (ImGui::BeginTabItem("Image"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Model"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(".mol"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}


		if (ImGui::Button("Export"))
		{
			//TODO : Implement File Dialog Opening so the user can select where they want to store/name the file  
			ChemVis::Exporter exporter;

			ChemVis::Visualiser selectVis = ChemVis::Visualiser2D;
			if (SelectedView == 1) { selectVis = ChemVis::Visualiser3D; }
			exporter.Config()->Visualiser = selectVis;
			exporter.Config()->Mode = ChemVis::Image;
			exporter.Export("");
		}
	}
	auto window = GetWindowData(true);
	ImGui::End();
	return window;
}


void InterfaceLayer::OnEvent(Core::Event& event)
{
}

WindowData InterfaceLayer::GetWindowData(bool closed, bool dockChange)
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

void InterfaceLayer::SetDarkStyle()
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

