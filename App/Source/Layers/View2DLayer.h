#pragma once

#include "Core/Layer.h"
#include "WindowData.h"

#include "Core/Application.h"
#include "ChemVis/Chemical.h"

class View2DLayer : public Core::Layer
{
public:
	View2DLayer();
	View2DLayer(std::shared_ptr<ChemVis::Chemical> chem);
	virtual ~View2DLayer();

	virtual void Update(float ts) override;
	virtual void OnRender() override;
	virtual void OnComposite() override;

	RenderTexture2D& getRenderTexture() { return m_Target; }

	void setWindowData(WindowData data) { m_WindowData = data; }

private:
	Camera2D m_Camera;
	RenderTexture2D m_Target = { 0 };
	Color m_ClearColor = BLACK;

	// Window Information
	Vector2 m_PrevSize = { 800, 800 };
	WindowData m_WindowData;
	bool m_Resizing = false;
	bool m_FirstFrame = true;
	bool m_ResizeQueued = false;
	
	std::shared_ptr<ChemVis::Chemical> m_Chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);
};