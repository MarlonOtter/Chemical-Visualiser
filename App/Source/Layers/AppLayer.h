#pragma once

#include "Core/Layer.h"
#include "raylib.h"

class AppLayer : public Core::Layer
{
public:
	AppLayer();
	virtual ~AppLayer();

	virtual void Update(float ts) override;
	virtual void OnComposite() override;

private:
	float textX = 0.0f;
};