#include "Settings.h"
#include <fstream>
#include <filesystem>

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
	Core::json json;

	json["FontSize"] = m_Values.FontSize;
	json["DarkMode"] = m_Values.DarkMode;
	json["TargetFPS"] = m_Values.TargetFPS;
	json["DynamicFramerate"] = m_Values.DynamicFramerate;
	
	json["AtomScale2D"] = m_Values.AtomScale2D;
	json["HydrogenScale2D"] = m_Values.HydrogenScale2D;
	json["BondWidth2D"] = m_Values.BondWidth2D;
	json["BondSeperation2D"] = m_Values.BondSeperation2D;
	json["WorldScale2D"] = m_Values.WorldScale2D;
	json["ShowElementLabels"] = m_Values.ShowElementLabels;
	json["LabelScale"] = m_Values.LabelScale;
	json["BackgroundColor2D"] = m_Values.BackgroundColor2D;
	json["CameraSmoothing2D"] = m_Values.CameraSmoothing2D;
	
	json["AtomScale3D"] = m_Values.AtomScale3D;
	json["HydrogenScale3D"] = m_Values.HydrogenScale3D;
	json["BondRadius3D"] = m_Values.BondRadius3D;
	json["BondSeperation3D"] = m_Values.BondSeperation3D;
	json["BondDetail3D"] = m_Values.BondDetail3D;
	json["BackgroundColor3D"] = m_Values.BackgroundColor3D;
	json["LookSensitivity3D"] = m_Values.LookSensitivity3D;
	json["PanSensitivity3D"] = m_Values.PanSensitivity3D;
	json["CameraSmoothing3D"] = m_Values.CameraSmoothing3D;
	
	json["ElementColors"] = m_Values.ElementColors;

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
	if (!std::filesystem::exists(FilePath)) return;

	std::string contents;
	std::ifstream inFile(FilePath);
	if (inFile.is_open())
	{
		std::stringstream buffer;
		buffer << inFile.rdbuf();
		contents = buffer.str();
		inFile.close();
	}
	ParseFromString(contents);
}

void Settings::ParseFromString(std::string data)
{
	try
	{
		Core::json json = Core::json::parse(data);

		ParseSetting(json, &m_Values.FontSize, "FontSize");
		ParseSetting(json, &m_Values.DarkMode, "DarkMode");
		ParseSetting(json, &m_Values.TargetFPS, "TargetFPS");
		ParseSetting(json, &m_Values.DynamicFramerate, "DynamicFramerate");
		ParseSetting(json, &m_Values.AtomScale2D, "AtomScale2D");
		ParseSetting(json, &m_Values.HydrogenScale2D, "HydrogenScale2D");
		ParseSetting(json, &m_Values.BondWidth2D, "BondWidth2D");
		ParseSetting(json, &m_Values.BondSeperation2D, "BondSeperation2D");
		ParseSetting(json, &m_Values.WorldScale2D, "WorldScale2D");
		ParseSetting(json, &m_Values.ShowElementLabels, "ShowElementLabels");
		ParseSetting(json, &m_Values.LabelScale, "LabelScale");
		ParseSetting(json, &m_Values.BackgroundColor2D, "BackgroundColor2D");
		ParseSetting(json, &m_Values.CameraSmoothing2D, "CameraSmoothing2D");
		ParseSetting(json, &m_Values.AtomScale3D, "AtomScale3D");
		ParseSetting(json, &m_Values.HydrogenScale3D, "HydrogenScale3D");
		ParseSetting(json, &m_Values.BondRadius3D, "BondRadius3D");
		ParseSetting(json, &m_Values.BondSeperation3D, "BondSeperation3D");
		ParseSetting(json, &m_Values.BondDetail3D, "BondDetail3D");
		ParseSetting(json, &m_Values.BackgroundColor3D, "BackgroundColor3D");
		ParseSetting(json, &m_Values.LookSensitivity3D, "LookSensitivity3D");
		ParseSetting(json, &m_Values.PanSensitivity3D, "PanSensitivity3D");
		ParseSetting(json, &m_Values.CameraSmoothing3D, "CameraSmoothing3D");
		ParseSetting(json, &m_Values.ElementColors, "ElementColors");
		
	}
	catch (const Core::json::parse_error& e)
	{
		// Failed To parse Settings.cfg, ignore and use defaults
		return;
	}
}

template<typename T>
void Settings::ParseSetting(Core::json& json, T* setting, std::string settingName)
{
	try
	{
		*setting = json.value<T>(settingName, *setting);
	}
	catch (...)
	{
		// Ignore errors and keep default value
	}

}

void Settings::SaveToDisk()
{
	std::string contents = Dump();
	std::ofstream outFile(FilePath, std::ios::trunc);
	if (outFile.is_open())
	{
		outFile << contents;
		outFile.close();
	}
	m_SaveQueued = false;
	m_Changed = false;
}

