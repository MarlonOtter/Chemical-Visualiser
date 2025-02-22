#include "Settings.h"

#include <iostream>
json Settings::settings;
bool Settings::changed = false;


int Settings::Save()
{
	// Turn the json to a string
	std::string settingString = settings.dump();
	// Add Formating to the file

	// Create The File
	return CreateFile("Settings.json", settingString);
}

int Settings::Refresh()
{
	// Check file exists
	// Otherwise read the settings from the backup source
	if (!std::filesystem::exists("Settings.json")) return ParseFile("SettingsDefault.json");

	// If it does get the contents
	return ParseFile("Settings.json");
}




std::vector<std::variant<std::string, Setting>> Settings::getChildren(std::string key)
{
	std::vector<std::variant<std::string, Setting>> returnVec;
	if (!settings.contains(json::json_pointer(key))) return returnVec;
	std::vector<std::string> branches = SearchBranch(settings.at(json::json_pointer(key)), "");

	// If the Branch being searched is an array
	if (settings.at(json::json_pointer(key)).is_array())
	{
		for (int i = 0; i < branches.size(); i++)
		{
			json item = settings.at(json::json_pointer(key + branches[i]));

			float Min = -1;
			float Max = -1;
			if (item.contains("Min"))
			{
				Min = item["Min"];
				Max = item["Max"];
			}

			Setting newSetting = CreateSetting(item);
			returnVec.push_back(newSetting);
		}

		return returnVec;
	}

	for (int i = 0; i < branches.size(); i++)
	{
		returnVec.push_back(branches[i]);
	}
	return returnVec;
}


int Settings::Set(std::string setting, std::string value)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;

	json& location = settings.at(ptr);
	if (location["Default"].type() != json::value_t::string) return -1;

	settings.at(ptr)["Value"] = value;
	changed = true;
}

int Settings::Set(std::string setting, int value)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;

	json& location = settings.at(ptr);
	if (location["Default"].type() != json::value_t::number_integer) return -1;

	settings.at(ptr)["Value"] = value;
	changed = true;
}

int Settings::Set(std::string setting, float value)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;

	json& location = settings.at(ptr);
	if (location["Default"].type() != json::value_t::number_float) return -1;

	settings.at(ptr)["Value"] = value;
	changed = true;
	return 0;
}

int Settings::Set(std::string setting, bool value)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;
	
	json& location = settings.at(ptr);
	if (location["Default"].type() != json::value_t::boolean) return -1;

	location["Value"] = value;
	changed = true;
}

int Settings::Set(std::string setting, glm::vec3 value)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;

	json& location = settings.at(ptr);
	if (location["Default"].type() != json::value_t::array) return -1;

	float col[3] = { value.x, value.y, value.z };
	settings.at(ptr)["Value"] = col;
	changed = true;
}



std::string Settings::GetType(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return "";

	return settings.at(ptr)["Type"];
}

Value Settings::GetValue(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return Value{};

	return CreateSetting(settings.at(ptr)).value;
}

Value Settings::GetDefault(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return Value{};

	return CreateSetting(settings.at(ptr)).defaultValue;
}

float Settings::GetMin(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return 0;
	
	json& location = settings.at(ptr);
	if (!location.contains("Min")) return 0;

	return location["Min"];
}

float Settings::GetMax(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return 0;
	
	json& location = settings.at(ptr);
	if (!location.contains("Max")) return 0;

	return location["Max"];
}



Setting Settings::GetAll(std::string setting)
{
	json::json_pointer ptr = Find(setting);

	// If it wasn't found return nothing
	if (ptr == json::json_pointer("")) return Setting{};
	
	json addr = settings.at(ptr);

	return CreateSetting(addr);
}


int Settings::Reset(std::string setting)
{
	json::json_pointer ptr = Find(setting);
	if (ptr == json::json_pointer("")) return -1;

	settings.at(ptr)["Value"] = settings.at(ptr)["Default"];
	return 0;
}

int Settings::ResetAll()
{
	std::vector<std::string> branches = SearchBranch(settings, "");

	for (int i = 0; i < branches.size(); i++)
	{
		json& branch = settings.at(branches[i]);
		if (!branch.is_array())
		{
			std::vector<std::string> moreBranches = SearchBranch(settings, branches[i]);
			branches.insert(branches.end(), moreBranches.begin(), moreBranches.end());
			continue;
		}

		for (int j = 0; j < branch.size(); j++)
		{
			branch["Value"] = branch["Default"];
		}
		
	}
	return 0;
}


std::string Settings::Dump()
{
	// Dump the entire json as a string
	return settings.dump();
}

std::string Settings::Dump(std::string setting)
{
	// Find the setting
	// Output the Json for that setting
	return "";
}




// Private

json::json_pointer Settings::Find(std::string setting)
{
	// Get a list of branches at the top level of the json tree 
	std::vector<std::string> branches = SearchBranch(settings, "");

	// Loop through each branch in the list
	for (int i = 0; i < branches.size(); i++)
	{
		json branch = settings.at(json::json_pointer(branches[i]));
		
		// If it is not an array it has non-settings children
		// Add those to the branches array
		if (!branch.is_array())
		{
			std::vector<std::string> moreBranches = SearchBranch(branch, branches[i]);
			branches.insert(branches.end(), moreBranches.begin(), moreBranches.end());
			continue;
		}
		// It is an array of settings
		// Loop through each setting checking whether it is what is being searched for
		for (int j = 0; j < branch.size(); j++)
		{
			// If the item in the array doesn't contain a Name there is an error and should be skipped
			if (!branch[j].contains("ID")) continue;

			// If the correct setting is found, return sits pointer
			if (branch[j]["ID"].get<std::string>() == setting) return json::json_pointer(branches[i] + "/" + std::to_string(j));
			
			// Otherwise move onto the next one
		}
	}

	// If it not found, return nothing
	return json::json_pointer("");
}

std::vector<std::string> Settings::SearchBranch(json jsonData, std::string ptr)
{
	// Go through each item at the top level
	std::vector<std::string> childBranches;
	int count = 0;
	for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it)
	{
		// Loop over every item in the json data at the top level
		
		// ERROR : cannot get a key from a list
		// Set the key as the index. This is if the json is an array
		std::string childKey = std::to_string(count);
		count++;

		// If the json is a json object get it key
		if (!jsonData.is_array()) childKey = it.key();

		// Add the key to the list.
		childBranches.push_back(ptr + "/" + childKey);
	}
	return childBranches;
}

std::string Settings::ReadFile(std::string file)
{
	std::cout << "READING FILE : " << file << "\n";
	// Read the contents of the file settings.json
	return get_file_contents(file.c_str());
}

int Settings::ParseFile(std::string file)
{
	std::string settingFileContents = ReadFile(file);

	// File is Empty return error
	if (settingFileContents == "") return -1;

	// Convert text to json object
	// Should Include Some Validation So that If it doesn't work it doesn't crash
	settings = json::parse(settingFileContents);
	return 0;
}

int Settings::InstallDefault()
{
	// Get Default Json
	std::string defaultJsonContent;
	// Put it in a new file
	return CreateFile("Settings.json", defaultJsonContent);

}

int Settings::CreateFile(std::string addr, std::string content)
{
	// If no address don't make a file
	if (addr == "") return -1;

	// Define the file name
	std::ofstream file(addr);

	// Add the content to the file
	file << content;

	// Create File
	file.close();
	std::cout << "WRITING FILE : " << addr << "\n";
	return 0;
}

Setting Settings::CreateSetting(json settingData)
{
	Value value;
	Value defaultValue;

	switch (settingData["Default"].type())
	{
		// I don't Handle these
	case json::value_t::null:
		return Setting{};
		break;

	case json::value_t::boolean:
		value.data = settingData["Value"].get<bool>();
		defaultValue.data = settingData["Default"].get<bool>();
		break;

	case json::value_t::number_integer:
		value.data = settingData["Value"].get<int>();
		defaultValue.data = settingData["Default"].get<int>();
		break;

		// I don't Handle these
	case json::value_t::number_unsigned:
		return Setting{};
		break;

	case json::value_t::number_float:
		value.data = settingData["Value"].get<float>();
		defaultValue.data = settingData["Default"].get<float>();
		break;

		// I don't Handle these
	case json::value_t::object:
		return Setting{};
		break;

	case json::value_t::array:
		float arr1[3];
		float arr2[3];
		for (int i = 0; i < settingData["Default"].size(); i++)
		{
			arr1[i] = settingData["Value"][i];
			arr2[i] = settingData["Default"][i];
		}
		value.data = glm::vec3(arr1[0],arr1[1], arr1[2]);
		defaultValue.data = glm::vec3(arr2[0], arr2[1], arr2[2]);
		break;

	case json::value_t::string:
		value.data = settingData["Value"].get<std::string>();
		defaultValue.data = settingData["Default"].get<std::string>();
		break;

		// I don't Handle these
	case json::value_t::binary:
		return Setting{};
		break;
	default:
		break;
	}


	float min = 0;
	float max = 0;
	if (settingData.contains("Min") && settingData.contains("Max"))
	{
		min = settingData["Min"];
		max = settingData["Max"];
	};

	return Setting{
		settingData["Type"],
		settingData["ID"],
		settingData["Name"],
		value,
		defaultValue,
		min,
		max
	};
}
