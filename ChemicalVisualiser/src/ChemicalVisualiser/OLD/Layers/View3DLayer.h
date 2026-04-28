#pragma once

#include "Slate/Core.h"
#include "Slate/Renderer.h"

#include "WindowData.h"
#include "ArcBallCamera.h"

#include "ChemVis/Chemical.h"

class View3DLayer : public Slate::Layer
{
public:
	View3DLayer();
	View3DLayer(std::shared_ptr<ChemVis::Chemical> chem);
	virtual ~View3DLayer();

	void OnUpdate(float ts) override;
	void OnRender() override;
	void OnComposite() override;

	Slate::RenderTexture& getRenderTexture() { return m_Target; }

	WindowData GetWindowData() { return m_WindowData; }
	void setWindowData(WindowData data) { m_WindowData = data; }
	
	ArcBallCamera& Camera() { return m_Camera; }
private:
	ArcBallCamera m_Camera;
	bool m_DebugCamera = false;
	Slate::RenderTexture m_Target = { 0 };

	// Window Information
	glm::vec2 m_PrevSize;
	bool m_Resizing = false;
	WindowData m_WindowData;
	bool m_FirstFrame = true;
	bool m_ResizeQueued = false;

	bool m_ForceRender = false;

	std::shared_ptr<ChemVis::Chemical> m_Chemical;

	void SetupRenderTexture();
	void ResetCamera(glm::vec3 Target = glm::vec3{ 0, 0, 0 });
	void HandleCameraMovement(float ts, glm::vec2 windowSize);
};