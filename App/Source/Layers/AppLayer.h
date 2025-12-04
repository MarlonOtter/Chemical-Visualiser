#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Core/Layer.h"
#include "ChemVis/PubChem.h"
#include "ChemVis/ChemicalList.h"
#include "ChemVis/FetchThreadHandler.h"
#include "Settings.h"

class AppLayer : public Core::Layer
{
public:
	AppLayer();
	virtual ~AppLayer();

	virtual void Update(float ts) override;
	virtual void OnComposite() override;
	virtual void OnEvent(Core::Event& event) override;

	bool IsCacheEmpty() const { return m_CacheEmpty; }
	std::map<std::string, int>& GetCache() { return m_CacheSnapshot; }
	void UpdateCacheSnapshot();

	void SetChemical(std::string chemical);
	Settings& GetSettings() { return m_Settings; }

	void RequestNewAutoComplete(std::string input) { m_AutoCompleteInput = input; }
	void QueueDeleteAllCachedChemicals() { m_DeleteAllCachedChemicals = true; }
	void QueueDeleteCachedChemical(int cid);
private:
	void HandleChemicalStructure();
	void HandleAutoComplete();
	void SendChemical(ChemVis::Chemical& chemical);

	void ManageFramerate(float ts);

	bool m_ChemicalRecieved = false;
	std::string m_Chemical;
	std::string m_CurrentlyDisplayed;

	std::string m_AutoCompleteInput;
	std::future<std::vector<std::string>> m_AutoCompleteFuture;
	bool m_AutoCompleteRequestActive = false;

	bool m_DeleteAllCachedChemicals = false;
	std::vector<int> m_DeleteQueue;

	bool m_CacheEmpty = true;
	std::map<std::string, int> m_CacheSnapshot;

	std::unique_ptr<ChemVis::FetchThread> m_FetchThread;
	Settings m_Settings;
};