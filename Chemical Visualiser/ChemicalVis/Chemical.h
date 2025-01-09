#pragma once

#include "nlohmann/json.hpp"


#include "Atom.h"

class Chemical 
{
public:
	//create a chemical object from data input
	Chemical();
	Chemical(std::string chemData);

	nlohmann::json json;

private:
	// CREATE CHEMICAL

	//this will take in the chemical data as input and spit out a json object
	//includes some validation
	void convertToJSON(std::string data);

	//get all the data about all the atoms and turn them into Atom Classes
	void parseAtoms(nlohmann::json json);

	//Get all the information about the chemical from the json data
	nlohmann::json parseChemicalInfo(nlohmann::json json);
};