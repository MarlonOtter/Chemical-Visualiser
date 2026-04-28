#include "InterfaceLayer.h"

#include "imgui.h"
#include "IconsFontAwesome6.h"

#include "ChemicalVisualiserLayer.h"

namespace ChemicalVisualiser
{
	static bool Startup = true;

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
		
	}

	InterfaceLayer::~InterfaceLayer()
	{
	}

	void InterfaceLayer::OnUpdate(float ts)
	{
		m_TimeSinceLastInput += ts;
	}

	void InterfaceLayer::OnImGuiRender()
	{
		if (Startup)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigWindowsMoveFromTitleBarOnly = true;
			SetDarkStyle();
			Startup = false;
		}

		DrawMenuBar();
		DrawDockSpace();

		DrawMainInterface();
		if (m_ShowSettings) DrawSettings();
		if (m_ShowCacheList) DrawCacheList();

		if (m_ShowDemo)
		{
			ImGui::ShowDemoWindow(&m_ShowDemo);
		}
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
				ImGui::BeginDisabled();
				ImGui::MenuItem("Export"); // TODO : Implement Export Functionality
				ImGui::MenuItem("Import"); // TODO : Implement Import Functionality
				ImGui::EndDisabled();

				if (ImGui::BeginMenu("Cache"))
				{
					Slate::Application& App = Slate::Application::Get();
					ChemicalVisualiserLayer& ChemicalVisualiser = *App.Layers().Get<ChemicalVisualiserLayer>();
					if (ImGui::MenuItem("View", nullptr, &m_ShowCacheList) && m_ShowCacheList)
					{
						ChemicalVisualiser.ChemicalList.GetList();
					}

					bool CacheEmpty = ChemicalVisualiser.ChemicalList.Size() == 0;
					if (CacheEmpty) ImGui::BeginDisabled();
					if (ImGui::MenuItem("Clear"))
					{
						// TODO : Confirm clear then send message to confirm that the cache has been cleared
						ChemicalVisualiser.ChemicalList.DeleteAll();
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
				ImGui::BeginDisabled();
				ImGui::MenuItem("Documentation"); // TODO : Link to online docs
				ImGui::MenuItem("Send Feedback"); // TODO : Link to feedback form
				ImGui::EndDisabled();
				ImGui::MenuItem("Dear ImGui Demo", nullptr, &m_ShowDemo);
				ImGui::BeginDisabled();
				ImGui::MenuItem("About"); // TODO : Opens an About Window
				ImGui::EndDisabled();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
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
					RequestNewAutoComplete(chemicalInp);
					m_MadeRequest = true;
				}
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_MAGNIFYING_GLASS) || entered)
			{
				//chemicalInp = std::string(buffer);
				entered = false;
				// send to app layer to fetch
				Slate::Application::Get().Layers().Get<ChemicalVisualiserLayer>()->SetChemical(chemicalInp);
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			{
				ImGui::SetTooltip("Search");
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

			ChemicalInformation& Info = Slate::Application::Get().Layers().Get<ChemicalVisualiserLayer>()->GetActiveChemicalAssetHandle().Get()->Chemical.ChemicalInformation;
			if (Info.CID != 0)
			{
				ImGui::NewLine();
				ImGui::SeparatorText("Chemical Information");

				ImGui::Text(
					std::format("Common Name: {}\nIUPAC Name: {}\nFormula: {}\nMass: {}\nCID: {}\nSMILES: {}\nInChI: {}",
						Info.CommonName, Info.IUPACName, Info.Formula, Info.Mass, Info.CID, Info.SMILES, Info.InChI).c_str()
				);
			}
		}
		window = getWindowData(!open);
		ImGui::End();
		return window;
	}


	WindowData InterfaceLayer::DrawSettings()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		Settings& settings = Slate::Application::Get().Layers().Get<ChemicalVisualiserLayer>()->Settings;
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

			if (ImGui::Button("Save")) settings.Save();

			ImGui::SameLine();

			if (ImGui::Button("Undo")) settings.QueueRevert();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) ImGui::SetTooltip("Reset To Last Saved Values");

			if (!SettingsChanged) ImGui::EndDisabled();

			ImGui::SameLine();

			if (ImGui::Button("Reset")) settings.Reset();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) ImGui::SetTooltip("Reset Values To Defaults");

			ImGuiIO& io = ImGui::GetIO();

			if (ImGui::BeginTabBar("SettingsTabs"))
			{
				if (ImGui::BeginTabItem("\xef\x83\x89 General ##SettingTab")) // Bars
				{
					// Use Bitwise OR so that all the UI is still displayed but I can detect if an entry is changed easily
					bool HasChanged = false;
					HasChanged |= ImGui::SliderFloat("Font Size##Global", &values.FontSize, 0.25f, 2.0f);
					HasChanged |= ImGui::Checkbox("Dark Mode##GlobalUI", &values.DarkMode);
					HasChanged |= ImGui::SliderInt("Target Framerate##Global", &values.TargetFPS, 15, 240);
					HasChanged |= ImGui::Checkbox("Dynamic Framerate##Global", &values.DynamicFramerate);
					HelpTooltip("Reduces performance when the program is inactive");

					// If dark mode setting has been reverted, it will also need to update any changes
					// So I can't have this in HasChanged
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

					if (HasChanged)
					{
						settings.MakeChange();
					}


					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("\xEF\x83\x88 2D ##SettingTab")) // Square
				{
					//glm::vec3 FloatBgColour = glm::vec3(values.BackgroundColor2D.r, values.BackgroundColor2D.g, values.BackgroundColor2D.b) / 255.0f;
					glm::vec3 FloatBgColour(1.0f);
					bool HasChanged = false;
					HasChanged |= ImGui::Checkbox("Background ##2D", &values.Background2D);
					HelpTooltip("Toggle Background Transparency For 2D View");
					// Only Draw the background colour edit if background is enabled
					HasChanged |= (values.Background2D && ImGui::ColorEdit3("Background Color ##2D", glm::value_ptr(FloatBgColour), ImGuiColorEditFlags_DisplayHex));
					HasChanged |= ImGui::SliderFloat("Atom Size ##2D", &values.AtomScale2D, 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Hydrogen Scale ##2D", &values.HydrogenScale2D, 0.01f, 1.0f);
					HasChanged |= ImGui::SliderFloat("Bond Width ##2D", &values.BondWidth2D, 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Bond Seperation ##2D", &values.BondSeperation2D, 0.01f, 2.0f);
					HasChanged |= ImGui::DragInt("World Scale ##2D", &values.WorldScale2D);
					HasChanged |= ImGui::Checkbox("Show Element Symbol ##2D", &values.ShowElementLabels);
					HasChanged |= ImGui::SliderFloat("Label Scale ##2D", &values.LabelScale, 0.01f, 0.5f);
					HasChanged |= ImGui::SliderFloat("Camera Smoothing ##2D", &values.CameraSmoothing2D, 0.0f, 1.0f);
					if (HasChanged)
					{
						settings.MakeChange();
						values.BackgroundColor2D = glm::u8vec3(FloatBgColour * 255.0f);
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("\xef\x86\xb2 3D ##SettingTab")) // Cube
				{
					//glm::vec3 FloatBgColour = glm::vec3(values.BackgroundColor3D.r, values.BackgroundColor3D.g, values.BackgroundColor3D.b) / 255.0f;
					glm::vec3 FloatBgColour(1.0f);
					bool HasChanged = false;
					HasChanged |= ImGui::Checkbox("Background ##3D", &values.Background3D);
					HelpTooltip("Toggle Background Transparency For 3D View");
					// Only Draw the background colour edit if background is enabled
					HasChanged |= (values.Background3D && ImGui::ColorEdit3("Background Color ##3D", glm::value_ptr(FloatBgColour), ImGuiColorEditFlags_DisplayHex));
					HasChanged |= ImGui::SliderFloat("Atom Size ##3D", &(values.AtomScale3D), 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Hydrogen Scale ##3D", &(values.HydrogenScale3D), 0.01, 1.0);
					HasChanged |= ImGui::SliderFloat("Bond Radius ##3D", &(values.BondRadius3D), 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Bond Detail ##3D", &(values.BondDetail3D), 0.0f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Bond Seperation ##3D", &(values.BondSeperation3D), 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Look Sensitivity ##3D", &(values.LookSensitivity3D), 0.0f, 3.0f);
					HasChanged |= ImGui::SliderFloat("Pan Sensitivity ##3D", &(values.PanSensitivity3D), 0.01f, 2.0f);
					HasChanged |= ImGui::SliderFloat("Camera Smoothing ##3D", &(values.CameraSmoothing3D), 0.0f, 1.0f);

					if (HasChanged)
					{
						settings.MakeChange();
						values.BackgroundColor3D = glm::u8vec3(FloatBgColour * 255.0f);
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Element Colours ##SettingTab"))
				{
					HelpTooltip("Change the colour that different elements display", false);
					for (size_t i = 0; i < ElementCount; i++)
					{
						//glm::vec3 FloatColour = glm::vec3(values.ElementColors[i].r, values.ElementColors[i].g, values.ElementColors[i].b) / 255.0f;
						glm::vec3 FloatColour(1.0f);
						std::string label = ChemicalVisualiser::GetElementSymbol(i + 1);

						bool HasChanged = false;
						HasChanged |= ImGui::ColorEdit3(label.c_str(), glm::value_ptr(FloatColour));
						if (HasChanged)
						{
							settings.MakeChange();
							values.ElementColors[i] = glm::u8vec3(FloatColour * 255.0f);
						}

					}

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}

		WindowData window = getWindowData(!open);
		ImGui::End();
		return window;
	}

	WindowData InterfaceLayer::DrawCacheList()
	{
		if (ImGui::Begin("\xef\x80\xba Cache", &m_ShowCacheList, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) // List
		{
			auto* Layer = Slate::Application::Get().Layers().Get<ChemicalVisualiserLayer>();
			auto Cache = Layer->ChemicalList.GetList();
			//Slate::Application::Get().GetLayer<AppLayer>()->UpdateCacheSnapshot();
			//auto cache = Slate::Application::Get().GetLayer<AppLayer>()->GetCache();

			ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersV | ImGuiTableFlags_SizingStretchProp;
			if (!Cache.empty() && ImGui::BeginTable("CacheList", 3, flags))
			{
				ImGui::TableSetupColumn("CID");
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("");
				ImGui::TableHeadersRow();

				for (const auto& [Name, Cid] : Cache)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::Text(std::to_string(Cid).c_str());
					ImGui::TableNextColumn();
					ImGui::Text(Name.c_str());
					ImGui::TableNextColumn();

					if (ImGui::Button((std::string("\xef\x8b\xad##Delete") + Name).c_str())) // Trash can
					{
						Layer->ChemicalList.Delete(Cid);
					}
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
					{
						ImGui::SetTooltip("Delete Chemical");
					}

					ImGui::SameLine();

					if (ImGui::Button((std::string("\xef\x82\x8e##Display") + Name).c_str())) //Arrow Up right from square
					{
						Layer->SetChemical(Name);
					}
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
					{
						ImGui::SetTooltip("Display Chemical");
					}
				}
				ImGui::EndTable();
			}

		}
		ImGui::End();
		return getWindowData(m_ShowCacheList, false);
	}

	void InterfaceLayer::OnEvent(Slate::EventDispatcher& Dispatcher)
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

	void InterfaceLayer::SetDarkStyle()
	{
		// Set style values (font DPI scaling and rounding)
		ImGuiStyle& style = ImGui::GetStyle();
		style.FontScaleDpi = ImGui::GetWindowDpiScale();
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

	void InterfaceLayer::HelpTooltip(std::string msg, bool SameLine)
	{
		if (SameLine) ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::SetTooltip(msg.c_str());
		}
	}

	void InterfaceLayer::RequestNewAutoComplete(std::string& str)
	{

	}
}