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

	void RequestNewAutoComplete(std::string input) { m_AutoCompleteInput = input; }

private:
	void DisplayChemicalStructure(std::string name);
	void GetAutoCompleteOptions();

	bool m_ChemicalRecieved = false;
	std::string m_Chemical;
	std::string m_AutoCompleteInput;
};