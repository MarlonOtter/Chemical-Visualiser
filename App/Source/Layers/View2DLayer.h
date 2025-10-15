#pragma once

#include "Core/Layer.h"

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

private:
	Camera2D camera;
	RenderTexture2D target;
	Color clearColor = BLACK;
	Vector2 viewportSize = { 800, 800 };
	Vector2 prevSize = { 800, 800 };
	bool hovered = false;

	std::shared_ptr<ChemVis::Chemical> chemical;

	void SetupRenderTexture();
	void ResetCamera();
	void HandleCameraMovement(float ts, Vector2 windowSize);

	
};