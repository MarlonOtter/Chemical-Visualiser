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

	float& AtomSize() { return m_AtomSize; }
	float& HydrogenScale() { return m_HydrogenScale; }
	float& BondRadius() { return m_BondRadius; }
	float& BondSeperation() { return m_BondSeperation; }
	float& BondDetail() { return m_BondDetail; }

private:
	Core::Camera::ArcBall m_Camera;
	bool m_DebugCamera = false;
	RenderTexture2D m_Target;
	Color m_ClearColor = BLACK;
	Vector2 m_PrevSize;
	bool m_Resizing = false;
	WindowData m_WindowData;

	std::shared_ptr<ChemVis::Chemical> m_Chemical;

	float m_AtomSize = 1.0f;
	float m_HydrogenScale = 0.5f;
	float m_BondRadius = 1.0f;
	float m_BondSeperation = 1.0f;
	float m_BondDetail = 1.0f;


	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};