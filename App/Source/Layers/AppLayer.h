#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Core/Layer.h"
#include "ChemVis/PubChem.h"
#include "ChemVis/ChemicalList.h"
#include "ChemVis/FetchThreadHandler.h"


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
	void QueueDeleteCachedChemicals() { m_DeleteCachedChemicals = true; }
private:
	void HandleChemicalStructure();
	void HandleAutoComplete();
	void SendChemical(ChemVis::Chemical& chemical);

	bool m_ChemicalRecieved = false;
	std::string m_Chemical;
	std::string m_CurrentlyDisplayed;

	std::string m_AutoCompleteInput;
	std::future<std::vector<std::string>> m_AutoCompleteFuture;
	bool m_AutoCompleteRequestActive = false;

	bool m_DeleteCachedChemicals = false;

	std::unique_ptr<ChemVis::FetchThread> m_FetchThread;
};