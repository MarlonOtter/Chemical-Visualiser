#include "ChemicalInputHandling.h"

std::future<std::string> CIH::thread;
bool CIH::makingRequest = false;
bool CIH::autoCompleteQueue = true;

int CIH::ValidateData(std::string& data)
{
	if (data.size() == 0) return -1;
	nlohmann::json json = nlohmann::json::parse(data);

	if (json.begin().key() == "Fault")
	{
		data = "";
		return -1;
	}
	return 0;
}

std::string CIH::GetData(std::string chemName)
{
	//get the normal data about the chemical (also contains 2D conformers)
	return PubChem::name(chemName);
}

std::string CIH::GetConformers(std::string chemName)
{
	return PubChem::conformers_name(chemName);
}

int CIH::queueAutoComplete()
{
	autoCompleteQueue = true;
	return 0;
}

int CIH::makeAutoCompleteRequest(std::string str)
{
	if (makingRequest || !autoCompleteQueue) return -1;
	autoCompleteQueue = false;
	try {
		//create thread
		thread = std::async(std::launch::async, PubChem::autoComplete, str, 5);
		makingRequest = true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in thread: " << e.what() << std::endl;
	}
	return 0;
}

int CIH::checkRequestProgress()
{
	if (!makingRequest) return -1;

	std::future_status status = thread.wait_for(std::chrono::seconds(0));
	if (status == std::future_status::ready)
	{
		makingRequest = false;
		return 1;
	}
	return 0;
}

std::string CIH::getRequestData()
{
	return thread.get();
}

bool CIH::isRequest()
{
	return makingRequest;
}