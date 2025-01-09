#pragma once
#include <string>
#include <vector>

#include <iostream>
#include <filesystem>
//for some reason can't use filesystem
//namespace fs = std::filesystem;

// I have made all these static to see how static works. I will only have one of these for the enitre
// thing so I don't need indiviidual objects of the same class

class ChemicalDataCache
{
public:
	//Path to the data files
	static std::string dataPath;
	//these are relative paths to the data Path
	static std::string cachePath;
	static std::string savedPath;

	//stored data
	static std::vector<std::string> names;
	static std::vector<int> CIDs;
	static std::vector<std::string> SMILEs;

	//Initalise the cache
	static int Init(std::string dataPath, std::string cachePath, std::string savedPath);

	//reset lists and get all the files
	static int Update();

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