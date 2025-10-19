#pragma once

#include "Core/Layer.h"

#include <optional>         
#include <functional>  
#include <string>

#include "raylib.h"	
#include "WindowData.h"
#include "ChemVis/Chemical.h"


class InterfaceLayer : public Core::Layer
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer();

	virtual void Update(float ts) override;
	virtual void OnComposite() override;
	virtual void OnEvent(Core::Event& event) override;

	void setRenderTextures(RenderTexture2D& rt2D, RenderTexture2D& rt3D) {
		renderTexture2D = rt2D;
		renderTexture3D = rt3D;
	}

	void SetChemicalInfo(ChemVis::ChemicalInfo info) { m_ChemicalInfo = info; }
	void PushError(std::string error);

	void SetAutoComplete(std::vector<std::string> options) { m_AutoCompleteOptions = options; }

private:
	std::optional<std::reference_wrapper<RenderTexture2D>> renderTexture2D;
	std::optional<std::reference_wrapper<RenderTexture2D>> renderTexture3D;

	WindowData window2D;
	WindowData window3D;

	ChemVis::ChemicalInfo m_ChemicalInfo;
	std::vector<std::string> m_AutoCompleteOptions;
	float m_TimeSinceLastInput = 0.0f;
	bool m_MadeRequest = false;

	bool m_ShowDemo = false;


	WindowData getWindowData(bool closed);

	void SetStyle();
	
	void DrawDockSpace();
	WindowData DrawView2D();
	WindowData DrawView3D();
	WindowData DrawMainInterface();
	WindowData DrawSettings();
};