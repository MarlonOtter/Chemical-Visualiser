#pragma once

#include "Core/Layer.h"
#include "raylib.h"
#include "raymath.h"

#include "Core/Application.h"

class View2DLayer : public Core::Layer
{
public:
	View2DLayer();
	virtual ~View2DLayer();

	virtual void Update(float ts) override;
	virtual void OnRender() override;
	virtual void OnComposite() override;

private:
	void HandleCameraMovement(float ts, Vector2 windowSize);

	Camera2D camera;
	RenderTexture2D target;
	Color clearColor = BLACK;
};