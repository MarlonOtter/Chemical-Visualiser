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
		void RequestDeleteCache(std::vector<int> cids);
		void RequestDeleteCacheAll();
		int GetCachedChemicalCount() const { return m_ChemicalList.Size(); }
		std::map<std::string, int> GetCachedListSnapshot() const;

		Chemical GetResult();
		bool IsResultReady();

		void Stop();
	private:
		void ThreadLoop();
		void FetchChemicalData(const std::string& name);
		std::thread m_Thread;
		std::atomic<bool> m_RunThread = { true };

		std::string m_ChemicalRequest;
		mutable std::mutex m_ChemicalRequestMutex;

		std::condition_variable m_ConditionVar;
		std::atomic<bool> m_NewRequest = { false };

		std::atomic<bool> m_DeleteCacheRequest = { false };
		std::atomic<bool> m_DeleteCacheAll = { false };
		mutable std::mutex m_DeleteQueueMutex;
		std::vector<int> m_DeleteQueue;

		mutable std::mutex m_ResultMutex;
		std::optional<Chemical> m_Result;

		mutable std::mutex m_ChemicalListMutex;
		ChemicalList m_ChemicalList;
	};
}