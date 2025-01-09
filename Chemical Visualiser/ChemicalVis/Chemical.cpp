#include "Chemical.h"

Chemical::Chemical(std::string chemData)
{
	convertToJSON(chemData);
}

void Chemical::convertToJSON(std::string chemData)
{
	json = nlohmann::json::parse(chemData);
}

void Chemical::parseAtoms(nlohmann::json json)
{
	json["PC_Compounds"][0]["atoms"]["aid"]; // this returns a list of the order in the elements array
	json["PC_Compounds"][0]["atoms"]["element"]; //this returns a list of elements to its atom 

	//returns 2D conformers
	json["PC_Compounds"][0]["coords"][0]["aid"];
	json["PC_Compounds"][0]["coords"][0]["conformers"][0]["x"];
	json["PC_Compounds"][0]["coords"][0]["conformers"][0]["y"];
	
}