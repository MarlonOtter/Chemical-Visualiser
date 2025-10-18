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

private:
	Core::Camera::ArcBall camera;
	bool DebugCamera = false;

	RenderTexture2D target;
	Color clearColor = BLACK;

	Vector2 prevSize;
	bool resizing = true;

	WindowData windowData;

	std::shared_ptr<ChemVis::Chemical> chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};