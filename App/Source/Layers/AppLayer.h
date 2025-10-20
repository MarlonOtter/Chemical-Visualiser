#pragma once

#include "Core/Layer.h"
#include "ChemVis/PubChem.h"
#include <string>
#include <vector>

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
	void HandleChemicalStructure();
	void HandleAutoComplete();

	bool m_ChemicalRecieved;
	std::string m_Chemical;
	std::string m_AutoCompleteInput;

	std::future<ChemVis::Chemical> m_StructureFuture;
	bool m_StructureRequestActive = false;

	std::future<std::vector<std::string>> m_AutoCompleteFuture;
	bool m_AutoCompleteRequestActive = false;
};