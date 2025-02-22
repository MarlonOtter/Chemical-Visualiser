#include "ChemicalDataCache.h"

const std::string ChemicalDataCache::dataPath = "C:/Users/marlo/OneDrive/Desktop/ChemicalVisualiser/ChemicalData/Cache";

const std::string ChemicalDataCache::cachePath;
const std::string ChemicalDataCache::savedPath;

std::vector<chemicalFile> ChemicalDataCache::files;

int ChemicalDataCache::Refresh()
{
	//loop through all the .json files in the chemical data directory
	for (const auto& file : std::filesystem::directory_iterator(dataPath))
	{
		if (!file.exists()) continue;
		if (file.is_directory()) continue;

		//validate each one has roughly correct data
		std::string path = dataPath + "/" + file.path().filename().string();
		std::cout << path << " : FILE PATH\n";
		std::string contents = get_file_contents(path.c_str());
		if (contents.size() == 0) continue;

		// Read relevent info from data 
		std::string name;
		std::string IUPAC;
		std::string SMILES;
		int CID;

		json entireJson = json::parse(contents);
		json data = entireJson["PC_Compounds"][0];

		CID = data["id"]["id"]["cid"].get<int>();

		json props = data["props"];

		for (int i = 0; i < props.size(); i++)
		{
			json value = props[i]["value"]["sval"];
			std::string label = props[i]["urn"]["label"].get<std::string>();
			if (!props[i]["urn"].contains("name")) continue;
			std::string name = props[i]["urn"]["name"].get<std::string>();

			if (label == "IUPAC Name")
			{
				if (name == "Systematic")
				{
					IUPAC = value.get<std::string>();
				}

				if (name == "Traditional")
				{
					name = value.get<std::string>();
				}
			}

			if (label == "SMILES" && name == "Absolute")
			{
				SMILES = value.get<std::string>();
			}

			if (IUPAC.size() > 0 && name.size() > 0 && SMILES.size() > 0) break;
		}

		//add relvent data to the cache class
		chemicalFile newfile =
		{
			path,
			name,
			IUPAC,
			SMILES,
			CID
		};
		files.push_back(newfile);

	}
	return 0;
}