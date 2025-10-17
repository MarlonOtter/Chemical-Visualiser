#pragma once

#include "Core/Layer.h"

#include <optional>         
#include <functional>  
#include <string>

#include "raylib.h"	
#include "WindowData.h"


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

	void PushError(std::string error);

private:
	std::optional<std::reference_wrapper<RenderTexture2D>> renderTexture2D;
	std::optional<std::reference_wrapper<RenderTexture2D>> renderTexture3D;

	WindowData getWindowData();

	WindowData window2D;
	WindowData window3D;
};