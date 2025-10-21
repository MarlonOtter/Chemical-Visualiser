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

		bool IsStored(std::string Identifier);
		bool IsStored(int Cid);

		void Store(std::string Identifier, int cid, std::string Data);
		std::string GetData(int Cid);
		int GetCid(std::string Identifier);
	private:
		// name/identifier -> cid
		std::map<std::string, int> m_Chemicals;
		std::string m_FileDirectory;

		bool FolderExists();
		bool FileExists(std::string FileName, bool prefix = true);
		std::string FileName(int Cid);

		void TrackStoredData();
		std::string FormatForFile(std::string Identifer, int cid, std::string Data);
		std::string ReadFile(std::string FilePath);
		bool WriteFile(std::string FilePath, std::string Contents);
	};

	static std::string Normalize(std::string s);
}