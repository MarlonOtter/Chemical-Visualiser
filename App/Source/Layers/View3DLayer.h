#pragma once

#include "Core/Layer.h"
#include "raylib.h"
#include "raymath.h"

#include "Core/Application.h"

class View3DLayer : public Core::Layer
{
public:
	View3DLayer();
	virtual ~View3DLayer();

	virtual void Update(float ts) override;
	virtual void OnRender() override;
	virtual void OnComposite() override;

private:
	void HandleCameraMovement(float ts, Vector2 windowSize);

	Camera3D camera;
	bool DebugCamera = false;

	RenderTexture2D target;
	Color clearColor = BLACK;
};