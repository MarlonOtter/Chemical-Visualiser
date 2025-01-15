#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

static class Settings
{
public:
	//stores all the settings in an easily parsed format
	static json settings;

	//set the value of a setting
	static int set(std::string setting, int value);

	// get the value of a setting
	static int get(std::string setting);

	//reset a setting to its default value
	static int reset(std::string setting);
};