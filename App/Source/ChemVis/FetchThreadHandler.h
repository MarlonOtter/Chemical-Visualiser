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
		void RequestDeleteCache() { 
			m_DeleteCacheRequest.store(true);
			m_ConditionVar.notify_one();
		}

		Chemical GetResult() { 
			std::optional<Chemical> ResultCopy;
			{
				std::lock_guard<std::mutex> lock(m_ResultMutex);
				ResultCopy = std::move(m_Result);
				m_Result.reset();
			}
			if (ResultCopy.has_value())
				return std::move(ResultCopy.value());
			return Chemical();
		}
		bool IsResultReady() 
		{
			std::lock_guard<std::mutex> lock(m_ResultMutex);
			return m_Result.has_value();
		}

		void Stop();
	private:
		void ThreadLoop();
		void FetchChemicalData(const std::string& name);
		std::thread m_Thread;
		std::atomic<bool> m_RunThread = { true };

		std::string m_ChemicalRequest;
		std::mutex m_ChemicalRequestMutex;

		std::condition_variable m_ConditionVar;
		std::atomic<bool> m_NewRequest = { false };
		std::atomic<bool> m_DeleteCacheRequest = { false };

		std::mutex m_ResultMutex;
		std::optional<Chemical> m_Result;


		ChemicalList m_ChemicalList;
	};
}