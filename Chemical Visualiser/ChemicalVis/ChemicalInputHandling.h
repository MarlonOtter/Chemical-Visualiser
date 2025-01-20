#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include "PubchemAccess.h"
#include <future>

// threading reference : 23 / 11 / 2024 
// https ://stackoverflow.com/questions/42418360/how-to-check-if-thread-has-finished-work-in-c11-and-above

class CIH
{
public:
	static std::future<std::string> thread;

	static int ValidateData(std::string& data);

	static std::string GetData(std::string chemName);
	static std::string GetConformers(std::string chemName);

	static int queueAutoComplete();
	static int makeAutoCompleteRequest(std::string str);

	static int checkRequestProgress();
	static std::string getRequestData();
	
	static bool isRequest();

private:
	
	static bool makingRequest;
	static bool autoCompleteQueue;
};