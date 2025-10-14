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
	virtual void OnEvent(Core::Event& event) override;

private:
	void DisplayChemicalStructure();

	float textX = 0.0f;
};