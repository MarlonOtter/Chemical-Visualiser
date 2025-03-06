#include "ChemicalInputHandling.h"

std::future<std::string> CIH::thread;
bool CIH::makingRequest = false;
bool CIH::autoCompleteQueue = true;

int CIH::ValidateData(std::string& data)
{
	if (data.size() == 0) return -1;
	
	try
	{
		nlohmann::json json = nlohmann::json::parse(data);
		if (json.begin().key() == "Fault")
		{
			data = "";
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR - exception: " << e.what() << "\n";
		data = "";
		return -1;
	};

	
	return 0;
}

chemicalData CIH::GetData(std::string chemInp, int type)
{
	if (type == 0)
	{
		chemicalData data;
		data.data = PubChem::name(chemInp);
		if (data.data.size() < 10) return chemicalData{ };
		data.conformers = PubChem::conformers_name(chemInp);
		return data;
	}
	else if (type == 1)
	{
		chemicalData data;
		int cid;

		// Validate the the input can be turned to a number
		
		try {
			cid = std::stoi(chemInp);
		}
		// If it can't output an error to the terminal
		// and return nothing
		catch (const std::exception err)
		{
			std::cout << "ERROR : Input is not a number\n";
			return { "", "" };
		} 
		
		data.data = PubChem::cid(cid);
		if (data.data.size() < 10) return chemicalData{ };
		data.conformers = PubChem::conformers_cid(cid);
		return data;
	}
	else if (type == 2)
	{
		chemicalData data;
		data.data = PubChem::structure(chemInp);
		if (data.data.size() < 10) return chemicalData{ };
		data.conformers = PubChem::conformers_struc(chemInp);
		return data;
	}
	std::cout << "ERROR: Invalid type: " << type << "\n";
	return { "", "" };
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