#pragma once
#include <string>
#include <vector>
#include "Renderer/shaderClass.h"

#include <iostream>
#include <filesystem>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct chemicalFile
{
	std::string filePath;
	std::string name;
	std::string IUPAC;
	std::string SMILES;
	int CID;
};

class ChemicalDataCache
{
public:
	//Path to the data files
	static const std::string dataPath;
	//these are relative paths to the data Path
	static const std::string cachePath;
	static const std::string savedPath;

	//stored data
	static std::vector<chemicalFile> files;

	//Initalise the cache
	static int Init(std::string dataPath, std::string cachePath, std::string savedPath);

	//reset lists and get all the files
	static int Refresh();

	//search for the data and return it 
	static std::string SearchName(std::string name);
	static std::string SearchCid(int CID);
	static std::string SearchStruct(std::string SMILE);

	static int ItemExists(std::string name);

	//list all chemicals that start with the provided prefix
	static std::vector<std::string> SearchPrefix(std::string prefix);
	
	//add a piece of data
	//It will handle adding the data to the lists
	static int Add(std::string data);

	//remove a piece of data
	static int RemoveName(std::string name);
	static int RemoveCid(int CID);
	static int RemoveStruct(std::string SMILE);

private:
	
};