#pragma once

#include "Core/Layer.h"
#include "raylib.h"
#include "raymath.h"

#include "Core/Application.h"
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

private:
	Camera3D camera;
	bool DebugCamera = false;

	RenderTexture2D target;
	Color clearColor = BLACK;

	std::shared_ptr<ChemVis::Chemical> chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};