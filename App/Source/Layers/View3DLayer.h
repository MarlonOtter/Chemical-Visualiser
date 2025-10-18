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

	RenderTexture2D& getRenderTexture() { return target; }

	void setWindowData(WindowData data) { windowData = data; }
	
	Core::Camera::ArcBall& Camera() { return camera; }

	float& AtomSize() { return m_AtomSize; }
	float& HydrogenScale() { return m_HydrogenScale; }
	float& BondRadius() { return m_BondRadius; }
	float& BondSeperation() { return m_BondSeperation; }
	int& BondDetail() { return m_BondDetail; }

private:
	Core::Camera::ArcBall camera;
	bool DebugCamera = false;
	RenderTexture2D target;
	Color clearColor = BLACK;
	Vector2 prevSize;
	bool resizing = true;
	WindowData windowData;

	std::shared_ptr<ChemVis::Chemical> chemical;

	float m_AtomSize = 0.25f;
	float m_HydrogenScale = 0.66f;
	float m_BondRadius = 0.1f;
	float m_BondSeperation = 0.1f;
	int m_BondDetail = 10;


	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};