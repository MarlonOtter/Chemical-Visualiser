#pragma once

#include "../Renderer/ShaderClass.h"
#include "nlohmann/json.hpp"
#include <variant>
#include <string>
#include <filesystem>
#include "glm/glm.hpp"

using json = nlohmann::json;

// All the possible datatypes that a setting can be stored in
struct Value {
	std::variant<int, float, std::string, bool, glm::vec3> data;
};

// Contains all the information about a setting
struct Setting {
	std::string type;
	std::string ID;
	std::string name;
	Value value;
	Value defaultValue;
	float min;
	float max;
};



class Settings
{
public:
	// This will be set to true if the settings are changed
	// It is used as a flag that will tell other parts of the program to refresh any data from settings
	// as it may have been updated.
	static bool changed;

	// Store the Settings JSON in an external file
	static int Save();
	// Read the File Settings.json from disk
	// and store it in memory
	static int Refresh();

	// Returns a list of keys of all the children of a parent json
	// If the child is a setting it will return the setting in the setting format
	static std::vector<std::variant<std::string, Setting>> getChildren(std::string key);

	// Set the value of a setting
	static int Set(std::string setting, std::string value);
	static int Set(std::string setting, int value);
	static int Set(std::string setting, float value);
	static int Set(std::string setting, bool value);
	static int Set(std::string setting, glm::vec3 value);
	
	// Get a piece of data about a setting
	static std::string GetType(std::string setting);
	static Value GetValue(std::string setting);
	static Value GetDefault(std::string setting);
	static float GetMin(std::string setting);
	static float GetMax(std::string setting);

	// Get all the information about a setting
	static Setting GetAll(std::string setting);

	// Reset a setting to its default value
	static int Reset(std::string setting);
	static int ResetAll();



	// Output the settings data as a string
	// All Data
	static std::string Dump();
	// Data about a singlular setting
	static std::string Dump(std::string setting);
	


private:
	//stores all the settings in an easily parsed format
	static json settings;

	// return a pointer to the desired setting
	static json::json_pointer Find(std::string setting);

	// creates a list of all child objects and returns their key
	static std::vector<std::string> SearchBranch(json jsonData, std::string ptr);

	
	// Read data from disk
	static std::string ReadFile(std::string file);
	// Turn string into Json object
	static int ParseFile(std::string file);

	// If the file doesn't exist, create it
	static int InstallDefault();

	// Create a File with provided content
	static int CreateFile(std::string addr, std::string content);

	// Turn a setting stored in JSON to a setting struct
	static Setting CreateSetting(json settingData);
};