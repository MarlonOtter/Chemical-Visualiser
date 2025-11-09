#include "Settings.h"
#include <fstream>
#include <filesystem>
#include "Core/Json.h"

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
	Core::json json = Core::json::parse(data);

	m_Values.FontSize = json.value<float>("FontSize", m_Values.FontSize);
	m_Values.DarkMode = json.value<bool>("DarkMode", m_Values.DarkMode);
	m_Values.TargetFPS = json.value<int>("TargetFPS", m_Values.TargetFPS);
	m_Values.DynamicFramerate = json.value<bool>("DynamicFramerate", m_Values.DynamicFramerate);

	m_Values.AtomScale2D = json.value<float>("AtomScale2D", m_Values.AtomScale2D);
	m_Values.HydrogenScale2D = json.value<float>("HydrogenScale2D", m_Values.HydrogenScale2D);
	m_Values.BondWidth2D = json.value<float>("BondWidth2D", m_Values.BondWidth2D);
	m_Values.BondSeperation2D = json.value<float>("BondSeperation2D", m_Values.BondSeperation2D);
	m_Values.WorldScale2D = json.value<int>("WorldScale2D", m_Values.WorldScale2D);
	m_Values.ShowElementLabels = json.value<bool>("ShowElementLabels", m_Values.ShowElementLabels);
	m_Values.LabelScale = json.value<float>("LabelScale", m_Values.LabelScale);
	m_Values.BackgroundColor2D = json.value<std::vector<int>>("BackgroundColor2D", m_Values.BackgroundColor2D);

	m_Values.AtomScale3D = json.value<float>("AtomScale3D", m_Values.AtomScale3D);
	m_Values.HydrogenScale3D = json.value<float>("HydrogenScale3D", m_Values.HydrogenScale3D);
	m_Values.BondRadius3D = json.value<float>("BondRadius3D", m_Values.BondRadius3D);
	m_Values.BondSeperation3D = json.value<float>("BondSeperation3D", m_Values.BondSeperation3D);
	m_Values.BondDetail3D = json.value<float>("BondDetail3D", m_Values.BondDetail3D);
	m_Values.BackgroundColor3D = json.value<std::vector<int>>("BackgroundColor3D", m_Values.BackgroundColor3D);
	m_Values.LookSensitivity3D = json.value<float>("LookSensitivity3D", m_Values.LookSensitivity3D);
	m_Values.PanSensitivity3D = json.value<float>("PanSensitivity3D", m_Values.PanSensitivity3D);
	m_Values.CameraSmoothing3D = json.value<float>("CameraSmoothing3D", m_Values.CameraSmoothing3D);

	m_Values.ElementColors = json.value<std::vector<int>>("ElementColors", m_Values.ElementColors);
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

