#pragma once

#include "nlohmann/json.hpp"

#include "Atom.h"

using json = nlohmann::json;

struct Bond
{
	int atomA;
	int atomB;
	int count;
};

class Chemical 
{
public:
	static const int _2D = 1;
	static const int _3D = 2;
	

	//create a chemical object from data input
	Chemical();
	Chemical(std::string chemData, std::string conformer3D);

	nlohmann::json data;

	std::vector<Atom> atoms;
	std::vector<Bond> bonds;

private:

	//Json format
	const std::string atomTypeAddr = "/PC_Compounds/0/atoms";
	const std::string atomPosAddr =  "/PC_Compounds/0/coords/0";
	const std::string bondsAddr = "/PC_Compounds/0/bonds";

	// CREATE CHEMICAL

	//this will take in the chemical data as input and spit out a json object
	//includes some validation
	void ConvertToJSON(std::string data);

	void AddConformer(std::string conformer3D);

	//get all the data about all the atoms and turn them into Atom Classes
	void ParseAtoms();

	void AddBonds();


	//Get all the information about the chemical from the json data
	nlohmann::json parseChemicalInfo();
};