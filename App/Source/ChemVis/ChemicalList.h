#pragma once

#include <map>
#include <string>

namespace ChemVis
{
	class ChemicalList
	{
	public:
		ChemicalList();
		ChemicalList(std::string Dir);
		~ChemicalList();

		bool IsStored(std::string Identifier) const;
		bool IsStored(int Cid) const;
		uint32_t Size() const { return m_Size; }

		void Store(std::string Identifier, int cid, std::string Data);
		std::string GetData(int Cid);
		int GetCid(std::string Identifier);

		void Delete(int cid);
		void DeleteAll();

		std::map<std::string, int> getList() const { return m_Chemicals; }
	private:
		// name/identifier -> cid
		std::map<std::string, int> m_Chemicals;
		std::string m_FileDirectory;

		bool FolderExists();
		bool FileExists(std::string FileName, bool prefix = true);
		std::string FileName(int Cid);
		uint32_t m_Size = 0;

		void TrackStoredData();
		std::string FormatForFile(std::string Identifer, int cid, std::string Data);
		std::string ReadFile(std::string FilePath);
		bool WriteFile(std::string FilePath, std::string Contents);
	};

	static std::string Normalize(std::string s);
}