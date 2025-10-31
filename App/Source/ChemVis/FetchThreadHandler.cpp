#include "FetchThreadHandler.h"
#include <iostream>

#include "PubChem.h"

namespace ChemVis
{
	FetchThread::FetchThread()
	{
		m_Thread = std::thread(&FetchThread::ThreadLoop, this);

		m_ChemicalList = ChemVis::ChemicalList("Cache/Chemicals");
	}
	FetchThread::~FetchThread()
	{
		std::cout << "Stopping Fetch Thread\n";
		Stop();
		if (m_Thread.joinable())
			m_Thread.join();
	}

	// Done by main thread
	void FetchThread::RequestChemical(const std::string& name)
	{
		{
			// lock while writing the request string
			std::lock_guard<std::mutex> lock(m_ChemicalRequestMutex);
			m_ChemicalRequest = name;
		}
		m_NewRequest.store(true);
	}

	void FetchThread::Stop()
	{
		m_RunThread.store(false);
	}

	// Done by fetch thread
	void FetchThread::ThreadLoop()
	{
		while (m_RunThread.load())
		{
			if (m_NewRequest.load())
			{
				std::string chemicalName;
				{
					std::lock_guard<std::mutex> lock(m_ChemicalRequestMutex);
					chemicalName = m_ChemicalRequest;
				}

				FetchChemicalData(chemicalName);
				m_NewRequest.store(false);
			}
			if (m_DeleteCacheRequest.load())
			{
				m_ChemicalList.DeleteAll();
				m_DeleteCacheRequest.store(false);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Should move to event driven (not polling)
		}
	}

	void FetchThread::FetchChemicalData(const std::string& name)
	{
		// Check if chemical is stored locally
		if (m_ChemicalList.IsStored(name))
		{
			std::cout << "READING FROM DISK\n";
			int cid = m_ChemicalList.GetCid(name);
			std::string data = m_ChemicalList.GetData(cid);
			auto chemical = ChemVis::Chemical::Parse(data);
			if (chemical.has_value())
			{
				{
					std::lock_guard<std::mutex> lock(m_ResultMutex);
					m_Result = chemical.value();
				}
				m_ResultReady.store(true);
			}
		}
		else
		{
			std::cout << "FETCHING FROM DB\n";
			auto m_StructureFuture = PubChem::Async::GetChemical(name);	
			try {
				auto result = m_StructureFuture.get();
				auto chemObj = result.Chemical;

				std::string ChemicalIdentifier = result.Identifier;
				if (!result.Data.empty() && !m_ChemicalList.IsStored(ChemicalIdentifier))
				{
					m_ChemicalList.Store(ChemicalIdentifier, std::stoi(chemObj.GetInfo().Cid), result.Data);
				}

				if (!chemObj.GetAtoms().Types.empty())
				{
					{
						std::lock_guard<std::mutex> lock(m_ResultMutex);
						m_Result = chemObj;
					}
					m_ResultReady.store(true);
				}
			}
			catch (const std::exception& e) {
				// Should push an error to the UI instead
				std::cerr << "Chemical request failed: " << e.what() << std::endl;
			}
		}
	}
}