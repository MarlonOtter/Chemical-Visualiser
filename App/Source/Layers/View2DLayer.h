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
	float& AtomSize() { return m_AtomSize; }
	float& HydrogenScale() { return m_HydrogenScale; }
	float& BondWidth() { return m_BondWidth; }
	float& BondSeperation() { return m_BondSeperation; }
	int& WorldScale() { return m_WorldScale; }
	bool& ShowSymbol() { return m_ShowElementSymbol; }

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

	// Visualisation Settings
	int m_WorldScale = 1000;
	float m_AtomSize = 1.0f;
	float m_HydrogenScale = 0.5f;
	float m_BondWidth = 1.0f;
	float m_BondSeperation = 1.0f;
	bool m_ShowElementSymbol = false;
	
	std::shared_ptr<ChemVis::Chemical> m_Chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);
};