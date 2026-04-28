#include "ChemicalList.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include <Slate/Json.h>
#include <Slate/Utils/File.h>

namespace ChemicalVisualiser
{
	static std::string NormaliseName(std::string s) 
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}

	ChemicalList::ChemicalList(const std::filesystem::path& path) : m_FileDirectory(path)
	{
		TrackStoredData();
	}

	ChemicalList::~ChemicalList()
	{
		//? should I create a file and stash m_Chemicals map
	}

	bool ChemicalList::IsStored(std::string Identifier)
	{
		bool value;
		{
			std::lock_guard lock(m_ListMutex);
			value = m_Chemicals.contains(NormaliseName(Identifier));
		}
		return value;
	}

	bool ChemicalList::IsStored(int Cid)
	{
		std::lock_guard lock(m_ListMutex);
		for (const auto& [key, value] : m_Chemicals) {
			if (value == Cid) {
				return true;
			}
		}
		return false;
	}

	void ChemicalList::Store(std::string Identifier, int CID, std::string Data)
	{
		if (IsStored(Identifier))
		{
			return;
		}

		std::filesystem::create_directories(m_FileDirectory);

		bool ReadComplete = false;
		std::string File = FileName(CID);
		std::string Path = m_FileDirectory.string() + "/" + File;
		if (std::filesystem::exists(Path))
		{
			// Update the contents to also contain the New Identifier
			std::string Contents = Slate::ReadFile(Path);
			try
			{
				Slate::json json = Slate::json::parse(Contents);
				std::vector<std::string> StoredIdentifiers = json["identifiers"].get<std::vector<std::string>>();
				
				bool Found = false;
				std::string NormalisedIdentifier = NormaliseName(Identifier);
				for (const auto& Id : StoredIdentifiers)
				{
					if (Id == NormalisedIdentifier)
					{
						Found = true;
						break;
					}
				}
				if (!Found)
				{
					StoredIdentifiers.push_back(NormalisedIdentifier);
					json["identifiers"] = StoredIdentifiers;
				}
				
				if (WriteFile(Path, json.dump())) ReadComplete = true;
			}
			catch (Slate::json::parse_error)
			{
				std::filesystem::remove(Path);
			}
		}
		if (!ReadComplete) {
			if (!WriteFile(Path, FormatForFile(Identifier, CID, Data))) return;
			m_Size++;
		}
		std::lock_guard lock(m_ListMutex);
		m_Chemicals[NormaliseName(Identifier)] = CID;
	}

	std::string ChemicalList::GetData(int CID)
	{
		std::string File = FileName(CID);
		std::filesystem::path Path(m_FileDirectory.string() + "/" + File);
		if (std::filesystem::exists(Path))
		{
			std::string Contents = Slate::ReadFile(Path);
			if (Contents.empty()) return "";
			try
			{
				Slate::json json = Slate::json::parse(Contents);
				if (!json.contains("cid") || !json.contains("identifiers") || !json.contains("data"))
				{
					std::cout << "MISSING DATA FROM FILE\n";
					return "";
				}
				if (json["cid"].get<int>() != CID)
				{
					std::filesystem::remove(Path);
					return "";
				}

				return json["data"].get<std::string>();
			}
			catch (Slate::json::parse_error)
			{
				std::cerr << "ERROR: failed to read data from file\n";
				std::filesystem::remove(Path);
				return "";
			}	
		}
		return "";
	}

	int ChemicalList::GetCid(std::string Identifier)
	{
		std::lock_guard lock(m_ListMutex);
		return m_Chemicals[NormaliseName(Identifier)];
	}

	void ChemicalList::DeleteAll()
	{
		if (!std::filesystem::exists(m_FileDirectory) || std::filesystem::is_empty(m_FileDirectory)) return;
		std::cout << "Removing All Files From: " << m_FileDirectory << "\n";
		for (const auto& entry : std::filesystem::directory_iterator(m_FileDirectory)) {
			if (entry.is_regular_file()) {
				std::filesystem::remove(entry.path());
			}
		}
		m_Size = 0;
		
		std::lock_guard lock(m_ListMutex);
		m_Chemicals.clear();
	}

	void ChemicalList::Delete(int CID)
	{
		if (!std::filesystem::exists(m_FileDirectory) || std::filesystem::is_empty(m_FileDirectory)) return;
		std::string Path = m_FileDirectory.string() + "/" + FileName(CID);
		if (!std::filesystem::exists(Path)) return;

		std::cout << "Removing File: " << Path << "\n";
		std::filesystem::remove(Path);

		std::lock_guard lock(m_ListMutex);
		for (auto it = m_Chemicals.begin(); it != m_Chemicals.end(); ) {
			if (it->second == CID) {
				it = m_Chemicals.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void ChemicalList::TrackStoredData()
	{
		if (!std::filesystem::exists(m_FileDirectory)) return;
		std::cout << "tracking Cache\n";
		for (const auto& entry : std::filesystem::directory_iterator(m_FileDirectory)) {
			if (entry.is_regular_file()) {
				std::string path = entry.path().string();
				std::string Contents = Slate::ReadFile(path);
				try
				{
					Slate::json json = Slate::json::parse(Contents);
					if (!json.contains("identifiers") || !json.contains("cid") || !json.contains("data")) continue;

					std::vector<std::string> identifiers = json["identifiers"].get<std::vector<std::string>>();
					int cid = json["cid"].get<int>();
					m_Size++;

					for (const std::string& id : identifiers)
					{
						std::lock_guard lock(m_ListMutex);
						m_Chemicals.emplace(NormaliseName(id), cid);
					}
				}
				catch (Slate::json::parse_error) {
					// maybe delete file?
					std::cerr << "failed to parse data on startup: " << path << "\n";
				}
				
			}
		}
	}

	std::string ChemicalList::FileName(int CID)
	{
		return std::to_string(CID) + ".json";
	}

	std::string ChemicalList::FormatForFile(std::string Identifer, int Cid, std::string Data)
	{
		Slate::json Result;
		Result["identifiers"] = std::vector<std::string>{ Identifer };
		Result["cid"] = Cid;
		Result["data"] = Data;
		return Result.dump();
	}
	
	bool ChemicalList::WriteFile(std::string FilePath, std::string Contents)
	{
		if (!std::filesystem::exists(m_FileDirectory)) return false;

		std::ofstream outFile(FilePath, std::ios::trunc);
		if (outFile.is_open())
		{
			outFile << Contents;
			outFile.close();
			return true;
		}
		return false;
	}

	
}
