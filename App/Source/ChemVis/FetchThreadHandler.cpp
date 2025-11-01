#include "FetchThreadHandler.h"
#include <iostream>

#include "PubChem.h"

namespace ChemVis
{
	FetchThread::FetchThread()
	{
		m_ChemicalList = ChemVis::ChemicalList("Cache/Chemicals");

		m_Thread = std::thread(&FetchThread::ThreadLoop, this);
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
			m_NewRequest = true;
		}
		m_ConditionVar.notify_one();	
	}

	void FetchThread::Stop()
	{
		m_RunThread.store(false);
		m_ConditionVar.notify_one();
	}

	// Done by fetch thread
	void FetchThread::ThreadLoop()
	{
		std::unique_lock<std::mutex> lock(m_ChemicalRequestMutex);
		while (m_RunThread.load())
		{
			m_ConditionVar.wait(lock, [this]() {
				return m_NewRequest || m_DeleteCacheRequest || !m_RunThread.load();
				});

			if (!m_RunThread.load())
				break;

			std::cout << "Thread Active\n";
			if (m_DeleteCacheRequest)
			{
				lock.unlock();
				m_ChemicalList.DeleteAll();
				lock.lock();
				m_DeleteCacheRequest.store(false);
				continue;
			}


			if (m_NewRequest.load())
			{
				std::string chemicalName = std::move(m_ChemicalRequest);
				m_ChemicalRequest.clear();
				m_NewRequest = false;
				lock.unlock();
				FetchChemicalData(chemicalName);
				lock.lock();
			}
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
					m_Result = std::move(chemical.value());
				}
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
						m_Result = std::move(chemObj);
					}
				}
			}
			catch (const std::exception& e) {
				// Should push an error to the UI instead
				std::cerr << "Chemical request failed: " << e.what() << std::endl;
			}
		}
	}
}