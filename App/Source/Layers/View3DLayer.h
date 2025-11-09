#pragma once

#include "Core/Layer.h"
#include "raylib.h"
#include "WindowData.h"
#include "Core/Cameras/ArcBall.h"

#include "ChemVis/Chemical.h"

class View3DLayer : public Core::Layer
{
public:
	View3DLayer();
	View3DLayer(std::shared_ptr<ChemVis::Chemical> chem);
	virtual ~View3DLayer();

	virtual void Update(float ts) override;
	virtual void OnRender() override;
	virtual void OnComposite() override;

	RenderTexture2D& getRenderTexture() { return m_Target; }

	WindowData GetWindowData() { return m_WindowData; }
	void setWindowData(WindowData data) { m_WindowData = data; }
	
	Core::Camera::ArcBall& Camera() { return m_Camera; }
private:
	Core::Camera::ArcBall m_Camera;
	bool m_DebugCamera = false;
	RenderTexture2D m_Target = { 0 };
	Color m_ClearColor = BLACK;

	// Window Information
	Vector2 m_PrevSize;
	bool m_Resizing = false;
	WindowData m_WindowData;
	bool m_FirstFrame = true;
	bool m_ResizeQueued = false;

	bool m_ForceRender = false;

	std::shared_ptr<ChemVis::Chemical> m_Chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);
};