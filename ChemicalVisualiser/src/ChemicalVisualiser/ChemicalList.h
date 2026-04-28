#pragma once

#include <unordered_map>
#include <string>
#include <mutex>
#include <filesystem>

namespace ChemicalVisualiser
{
	// TODO Make Thread Safe
	class ChemicalList
	{
	public:
		ChemicalList(const std::filesystem::path& path);
		~ChemicalList();

		bool IsStored(std::string Identifier);
		bool IsStored(int Cid);
		uint32_t Size() const { return m_Size; }

		void Store(std::string Identifier, int cid, std::string Data);
		std::string GetData(int Cid);
		int GetCid(std::string Identifier);

		void Delete(int cid);
		void DeleteAll();

		std::unordered_map<std::string, int> GetList() 
		{
			std::lock_guard lock(m_ListMutex);
			return m_Chemicals;
		}
	private:
		std::string FormatForFile(std::string Identifer, int cid, std::string Data);
		void TrackStoredData(); 
		std::string FileName(int CID);
		bool WriteFile(std::string FilePath, std::string Contents);
	private:
		std::mutex m_ListMutex;
		// name/identifier -> cid
		std::unordered_map<std::string, int> m_Chemicals;
		const std::filesystem::path m_FileDirectory;
		uint32_t m_Size = 0;

	};

	//static std::string Normalize(std::string s);
}