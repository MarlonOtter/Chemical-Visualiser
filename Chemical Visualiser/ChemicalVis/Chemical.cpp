#include "Chemical.h"

Chemical::Chemical()
{

}

Chemical::Chemical(std::string chemData)
{
	ConvertToJSON(chemData);
	ParseAtoms();
	AddBonds();
}

void Chemical::ConvertToJSON(std::string chemData)
{
	data = nlohmann::json::parse(chemData);
}

void Chemical::ParseAtoms()
{
	json atomTypeJson = data.at(json::json_pointer(atomTypeAddr));

	atomTypeJson["aid"]; // this returns a list of the order in the elements array
	atomTypeJson["element"]; //this returns a list of elements to its atom 

	//returns 2D conformers
	json atomPosJson = data.at(json::json_pointer(atomPosAddr));


	int numberOfAtoms = atomPosJson["aid"].size();
	atoms.assign(numberOfAtoms, Atom(glm::vec3(0), 0));
	for (int i = 0; i < numberOfAtoms; i++)
	{
		int index = atomPosJson["aid"][i];
		float x = atomPosJson["conformers"][0]["x"][i];
		float y = atomPosJson["conformers"][0]["y"][i];

		//TODO: Take into consideration the order of atomType from the aid array
		Atom atom(glm::vec3(x, y, 0.0f), atomTypeJson["element"][i]);
		atoms[index - 1] = atom;
	}
	
	return;
}

void Chemical::AddBonds()
{
	json bondsJson = data.at(json::json_pointer(bondsAddr));
	int numBonds = bondsJson["aid1"].size();
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

		bonds.push_back(Bond{ atomA, atomB, static_cast<unsigned int>(order) });
	}
}

