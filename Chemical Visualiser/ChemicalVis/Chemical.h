#pragma once

#include "nlohmann/json.hpp"

#include "Atom.h"

using json = nlohmann::json;

class Chemical 
{
public:
	//create a chemical object from data input
	Chemical();
	Chemical(std::string chemData, Model& model, Shader& shader);

	nlohmann::json data;
	std::vector<Atom> atoms;

	Model* atomModel;
	Shader* atomShader;

	void Draw(Camera& camera);

private:

	//Json format
	std::string atomTypeAddr = "/PC_Compounds/0/atoms";
	std::string atomPosAddr =  "/PC_Compounds/0/coords/0";

	// CREATE CHEMICAL

	//this will take in the chemical data as input and spit out a json object
	//includes some validation
	void convertToJSON(std::string data);

	//get all the data about all the atoms and turn them into Atom Classes
	void parseAtoms();

	//Get all the information about the chemical from the json data
	nlohmann::json parseChemicalInfo();
};