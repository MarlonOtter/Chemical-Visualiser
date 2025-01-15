#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include "PubchemAccess.h"
#include <future>

class CIH
{
public:
	

	static int ValidateData(std::string& data);

	static std::string GetData(std::string chemName);

	static int queueAutoComplete();
	static int makeAutoCompleteRequest(std::string str);

	static int checkRequestProgress();
	static std::string getRequestData();
	
	static bool isRequest();

private:
	static std::future<std::string> thread;
	static bool makingRequest;
	static bool autoCompleteQueue;
};