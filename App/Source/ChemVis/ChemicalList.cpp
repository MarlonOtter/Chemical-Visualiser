#include "ChemicalList.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "Core/Json.h"
#include <algorithm>

namespace ChemVis
{
	ChemicalList::ChemicalList()
	{

	}

	ChemicalList::ChemicalList(std::string Dir) : m_FileDirectory(Dir)
	{
		TrackStoredData();
	}

	ChemicalList::~ChemicalList()
	{
		// create a file and stash m_Chemicals map
	}

	bool ChemicalList::IsStored(std::string Identifier)
	{
		return m_Chemicals.contains(Normalize(Identifier));
	}

	bool ChemicalList::IsStored(int Cid)
	{
		for (const auto& [key, value] : m_Chemicals) {
			if (value == Cid) {
				return true;
			}
		}
		return false;
	}

	void ChemicalList::Store(std::string Identifier, int Cid, std::string Data)
	{
		if (IsStored(Cid))
		{
			m_Chemicals[Identifier] = Cid;
			return;
		}

		std::filesystem::create_directories(m_FileDirectory);

		bool ReadComplete = false;
		std::string FilePath = m_FileDirectory + "/" + FileName(Cid);
		if (FileExists(FileName(Cid)))
		{
			std::string contents = ReadFile(FilePath);
			try
			{
				Core::json json = Core::json::parse(contents);
				std::vector<std::string> identifiers = json["identifiers"].get<std::vector<std::string>>();
				identifiers.push_back(Identifier);
				json["identifiers"] = identifiers;

				if (WriteFile(FilePath, json.dump())) ReadComplete = true;
			}
			catch (Core::json::parse_error)
			{
				std::filesystem::remove(FilePath);
			}
		}
		if (!ReadComplete) {
			if (!WriteFile(FilePath, FormatForFile(Identifier, Cid, Data))) return;
		}
		m_Chemicals[Identifier] = Cid;
	}

	std::string ChemicalList::GetData(int Cid)
	{
		if (FileExists(FileName(Cid)))
		{
			std::string FilePath = m_FileDirectory + "/" + FileName(Cid);
			std::string Contents = ReadFile(FilePath);
			if (Contents.empty()) return "";
			try
			{
				Core::json json = Core::json::parse(Contents);
				if (!json.contains("cid") || !json.contains("identifiers") || !json.contains("data"))
				{
					std::cout << "MISSING DATA FROM FILE\n";
					return "";
				}
				if (json["cid"].get<int>() != Cid)
				{
					std::filesystem::remove(FilePath);
					return "";
				}

				return json["data"].get<std::string>();
			}
			catch (Core::json::parse_error)
			{
				std::cerr << "ERROR: failed to read data from file\n";
				std::filesystem::remove(FilePath);
				return "";
			}	
		}
		return "";
	}

	int ChemicalList::GetCid(std::string Identifier)
	{
		return m_Chemicals[Normalize(Identifier)];
	}

	void ChemicalList::TrackStoredData()
	{
		if (!FolderExists()) return;
		std::cout << "tracking Cache\n";
		for (const auto& entry : std::filesystem::directory_iterator(m_FileDirectory)) {
			if (entry.is_regular_file()) {
				std::string path = entry.path().string();
				std::string Contents = ReadFile(path);
				try
				{
					Core::json json = Core::json::parse(Contents);
					if (!json.contains("identifiers") || !json.contains("cid") || !json.contains("data")) continue;

					std::vector<std::string> identifiers = json["identifiers"].get<std::vector<std::string>>();
					int cid = json["cid"].get<int>();

					for (std::string id : identifiers)
					{
						m_Chemicals.try_emplace(Normalize(id), cid);
					}
				}
				catch (Core::json::parse_error) {
					// maybe delete file?
					std::cerr << "failed to parse data on startup: " << path << "\n";
				}
				
			}
		}
	}

	std::string ChemicalList::FileName(int Cid)
	{
		return std::to_string(Cid) + ".json";
	}

	bool ChemicalList::FolderExists()
	{
		return std::filesystem::exists(m_FileDirectory);
	}

	bool ChemicalList::FileExists(std::string FileName, bool prefix)
	{
		if (prefix) return std::filesystem::exists(m_FileDirectory + "/" + FileName);
		else return std::filesystem::exists(FileName);
	}

	std::string ChemicalList::FormatForFile(std::string Identifer, int Cid, std::string Data)
	{
		Core::json Result;
		Result["identifiers"] = std::vector<std::string>{ Identifer };
		Result["cid"] = Cid;
		Result["data"] = Data;
		return Result.dump();
	}
	
	std::string ChemicalList::ReadFile(std::string File)
	{
		if (!FileExists(File, false)) return "";

		std::ifstream inFile(File);
		if (inFile.is_open())
		{
			std::stringstream buffer;
			buffer << inFile.rdbuf();
			std::string contents = buffer.str();
			inFile.close();
			return contents;
		}
		return "";
	}

	bool ChemicalList::WriteFile(std::string FilePath, std::string Contents)
	{
		if (!FolderExists()) return false;

		std::ofstream outFile(FilePath, std::ios::trunc);
		if (outFile.is_open())
		{
			outFile << Contents;
			outFile.close();
			return true;
		}
		return false;
	}

	std::string Normalize(std::string s) {
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}
}
