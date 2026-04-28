#include "Settings.h"
#include <fstream>
#include <filesystem>
#include <iostream>

#include <Slate/Utils/File.h>

namespace ChemicalVisualiser
{
	Settings::Settings()
	{
		ResetToDefaults();
		ReadFromDisk();
	}

	void Settings::ResetToDefaults()
	{
		m_Values = SettingValues();
	}

	std::string Settings::Dump() const
	{
		// TODO Check that this doesn't throw an error if the data is invalid
		Slate::json json = m_Values;
		return json.dump(4);
	}

	void Settings::Save()
	{
		if (!m_Changed) return;
		m_SaveQueued = true;
	}

	void Settings::QueueRevert()
	{
		if (!m_Changed) return;
		m_RevertQueued = true;
	}

	void Settings::Revert()
	{
		ReadFromDisk();
		m_Changed = false;
		m_RevertQueued = false;
	}

	void Settings::Reset()
	{
		ResetToDefaults();
		MakeChange();
	}

	void Settings::ReadFromDisk()
	{
		std::string Contents = Slate::ReadFile(m_Path);
		ParseFromString(Contents);
	}

	void Settings::ParseFromString(std::string data)
	{
		try
		{
			Slate::json json = Slate::json::parse(data);
			// TODO Check that this doesn't throw an error if the data is invalid
			m_Values = json.get<SettingValues>();
		}
		catch (const Slate::json::parse_error& e)
		{
			std::cout << "Failed To parse Settings.cfg, using defaults\n";
			return;
		}
	}

	void Settings::SaveToDisk()
	{
		std::string contents = Dump();
		std::ofstream outFile(m_Path, std::ios::trunc);
		if (outFile.is_open())
		{
			outFile << contents;
			outFile.close();
		}
		m_SaveQueued = false;
		m_Changed = false;
	}

}