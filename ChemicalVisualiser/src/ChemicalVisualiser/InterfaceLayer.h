#pragma once

#include "Slate/Core.h"

#include <optional>         
#include <functional>  
#include <string>

#include "WindowData.h"
#include "Chemical.h"

namespace ChemicalVisualiser
{
	class InterfaceLayer : public Slate::Layer
	{
	public:
		InterfaceLayer();
		virtual ~InterfaceLayer();

		void OnUpdate(float ts) override;
		void OnImGuiRender() override;
		void OnEvent(Slate::EventDispatcher& Dispactcher) override;
		void PushError(std::string Error);

		void SetAutoComplete(std::vector<std::string> options) { m_AutoCompleteOptions = options; }

	private:
		void RequestNewAutoComplete(std::string& str);

		WindowData getWindowData(bool closed, bool dockChange = false);

		void SetDarkStyle();
	
		void DrawDockSpace();

		void DrawMenuBar();
		WindowData DrawMainInterface();
		WindowData DrawSettings();
		WindowData DrawCacheList();

		void HelpTooltip(std::string msg, bool SameLine = true);

	private:
		std::vector<std::string> m_AutoCompleteOptions;
		float m_TimeSinceLastInput = 0.0f;
		bool m_MadeRequest = false;

		bool m_ShowDemo = false;
		bool m_ShowSettings = false;
		bool m_ShowCacheList = false;

		bool m_PreviousDarkMode = true;
	};
}