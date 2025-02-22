#include "Chemical.h"

Chemical::Chemical()
{

}

Chemical::Chemical(std::string chemData, std::string conformer3D)
{

	ConvertToJSON(chemData);
	AddConformer(conformer3D);
	ParseAtoms();
	AddBonds();
}

void Chemical::ConvertToJSON(std::string chemData)
{
	data = nlohmann::json::parse(chemData);
}

void Chemical::AddConformer(std::string conformer3D)
{
	// if there is no 3D structure given return as there is no data to add 
	if (conformer3D.size() == 0) return;
	json conformerJson = json::parse(conformer3D);
	json conformerData = conformerJson.at(json::json_pointer(atomPosAddr))["conformers"][0];

	//if the data being put in has no z, don't add it
	if (!conformerData.contains("z")) return;

	//insert the data
	data.at(json::json_pointer(atomPosAddr))["conformers"][1] = conformerData;
}


void Chemical::ParseAtoms()
{
	json atomTypeJson = data.at(json::json_pointer(atomTypeAddr));

	//returns 2D conformers
	json atomPosJson = data.at(json::json_pointer(atomPosAddr));


	int numberOfAtoms = static_cast<int>(atomPosJson["aid"].size());
	atoms.assign(numberOfAtoms, Atom(0));
	for (int i = 0; i < numberOfAtoms; i++)
	{
		//get the x and y conformers from the data
		int index = atomPosJson["aid"][i];


		//get 2D structure data
		glm::vec2 pos2D(0);
		pos2D.x = atomPosJson["conformers"][0]["x"][i];
		pos2D.y = atomPosJson["conformers"][0]["y"][i];


		//if there is 3D data get it.
		glm::vec3 pos3D = glm::vec3(pos2D, 0.0f);
		if (atomPosJson["conformers"].size() > 1)
		{
			pos3D.x = atomPosJson["conformers"][1]["x"][i];
			pos3D.y = atomPosJson["conformers"][1]["y"][i];
			pos3D.z = atomPosJson["conformers"][1]["z"][i];
		}

		Atom atom(atomTypeJson["element"][i], pos3D, pos2D);
		atoms[index - 1] = atom;
	}
	return;
}

void Chemical::AddBonds()
{
	if (!data["PC_Compounds"][0].contains("bonds")) return;
	json bondsJson = data.at(json::json_pointer(bondsAddr));

	//if the json doesn't contains bond data, return
	//if (!bondsJson.contains("aid1")) return;
	
	//loop through each bond and add it to the list
	int numBonds = static_cast<int>(bondsJson["aid1"].size());
	for (int i = 0; i < numBonds; i++)
	{
		bool skip = false;
		//the atoms in the bond
		int atomA = bondsJson["aid1"][i];
		int atomB = bondsJson["aid2"][i];

		// the number of bonds
		int order = bondsJson["order"][i];

		//if the bond has already been added to the list skip
		if (skip) continue;

		bonds.push_back(Bond{ atomA, atomB, static_cast<int>(order) });
	}
}

