#pragma once

#include "Core/Layer.h"
#include <string>

class AppLayer : public Core::Layer
{
public:
	AppLayer();
	virtual ~AppLayer();

	virtual void Update(float ts) override;
	virtual void OnComposite() override;
	virtual void OnEvent(Core::Event& event) override;


	void SetChemical(std::string chemical);
private:
	void DisplayChemicalStructure(std::string name);

	bool chemicalRecieved = false;
	std::string chemical;

	float textX = 0.0f;
};