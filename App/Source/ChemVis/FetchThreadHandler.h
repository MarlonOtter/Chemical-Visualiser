#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>

#include "Chemical.h"
#include "ChemicalList.h"

namespace ChemVis
{
	class FetchThread
	{
	public:
		FetchThread();
		~FetchThread();
		void RequestChemical(const std::string& name);
		void RequestDeleteCache() { m_DeleteCacheRequest.store(true); }

		Chemical GetResult() { 
			Chemical ResultCopy;
			{
				std::lock_guard<std::mutex> lock(m_ResultMutex);
				ResultCopy = m_Result;
			}
			m_ResultReady.store(false);
			return ResultCopy;
		}
		bool IsResultReady() { return m_ResultReady.load(); }

		void Stop();
	private:
		void ThreadLoop();
		void FetchChemicalData(const std::string& name);
		std::thread m_Thread;
		std::atomic<bool> m_RunThread = { true };

		std::string m_ChemicalRequest;
		std::mutex m_ChemicalRequestMutex;

		std::atomic<bool> m_NewRequest = { false };
		std::atomic<bool> m_DeleteCacheRequest = { false };

		std::atomic<bool> m_ResultReady = { false };
		std::mutex m_ResultMutex;
		Chemical m_Result;


		ChemicalList m_ChemicalList;
	};
}