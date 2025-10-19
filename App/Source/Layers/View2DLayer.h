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

	RenderTexture2D& getRenderTexture() { return target; }
	float& AtomSize() { return m_AtomSize; }
	float& HydrogenScale() { return m_HydrogenScale; }
	float& BondWidth() { return m_BondWidth; }
	float& BondSeperation() { return m_BondSeperation; }
	int& WorldScale() { return m_WorldScale; }

	void setWindowData(WindowData data) { windowData = data; }

private:
	
	Camera2D camera;
	RenderTexture2D target;
	Color clearColor = BLACK;
	Vector2 viewportSize = { 800, 800 };
	Vector2 prevSize = { 800, 800 };
	
	WindowData windowData;
	bool hovered = false;

	bool resizing = true;

	int m_WorldScale = 1000;
	float m_AtomSize = 0.25f;
	float m_HydrogenScale = 0.5f;
	float m_BondWidth = 0.1f;
	float m_BondSeperation = 0.2f;


	std::shared_ptr<ChemVis::Chemical> chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};