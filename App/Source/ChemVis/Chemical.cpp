#include "Chemical.h"
#include <iostream>

namespace ChemVis 
{
	Chemical::Chemical()
	{
	}

	Chemical::Chemical(const AtomsInfo atoms, const BondsInfo bonds, const ChemicalInfo info)
		: m_Info(info), m_Atoms(atoms), m_Bonds(bonds)
	{
	}

	Chemical::~Chemical()
	{
	}

	Chemical Chemical::Parse(std::string data)
	{
		Core::json DataJson = Core::json::parse(data);

		return Chemical(ParseAtoms(DataJson), ParseBonds(DataJson), ParseInfo(DataJson));
	}

	AtomsInfo Chemical::ParseAtoms(Core::json data)
	{
		const std::string AtomTypeAddr = "/PC_Compounds/0/atoms";
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0";

		Core::json AtomTypeJson = data.at(Core::json::json_pointer(AtomTypeAddr));
		Core::json AtomPosJson = data.at(Core::json::json_pointer(AtomPosAddr));

		AtomsInfo atoms = {};
		atoms.Types = AtomTypeJson["element"].get<std::vector<int>>();

		atoms.Positions2D.x = AtomPosJson["conformers"][0]["x"].get<std::vector<float>>();
		atoms.Positions2D.y = AtomPosJson["conformers"][0]["y"].get<std::vector<float>>();
		size_t AtomCount = AtomPosJson["aid"].size();
		atoms.Positions2D.z = std::vector<float>(AtomCount, 0.0f); //2D structure has no z

		//std::cout << "Conformers: \n" << AtomPosJson["conformers"].dump() << "\n";
		if (AtomPosJson["conformers"].size() > 1) {
			atoms.Positions3D.x = AtomPosJson["conformers"][1]["x"].get<std::vector<float>>();
			atoms.Positions3D.y = AtomPosJson["conformers"][1]["y"].get<std::vector<float>>();
			atoms.Positions3D.z = AtomPosJson["conformers"][1]["z"].get<std::vector<float>>();
		}

		return atoms;
	}

	BondsInfo Chemical::ParseBonds(Core::json data)
	{
		const std::string BondsAddr = "/PC_Compounds/0/bonds";

		if (!data["PC_Compounds"][0].contains("bonds")) return BondsInfo{};
		Core::json bondsJson = data.at(Core::json::json_pointer(BondsAddr));

		BondsInfo bonds = {};
		bonds.BeginAtomIndices = bondsJson["aid1"].get<std::vector<int>>();
		bonds.EndAtomIndices = bondsJson["aid2"].get<std::vector<int>>();
		bonds.BondOrders = bondsJson["order"].get<std::vector<int>>();

		return bonds;
	}

	ChemicalInfo Chemical::ParseInfo(Core::json data)
	{
		
		

		const std::string NameAddr = "/PC_Compounds/0/props/6/value/sval";



		return ChemicalInfo{
			data.at(Core::json::json_pointer(NameAddr)).get<std::string>()
		};

		//! need to check this
		return ChemicalInfo{ data["PC_Compounds"][0]["title"].get<std::string>(),
							 data["PC_Compounds"][0]["molecular_formula"].get<std::string>(),
							 data["PC_Compounds"][0]["molecular_weight"].get<float>() };
	}


	std::string Merge2Dand3D(std::string data2D, std::string data3D)
	{
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0/conformers";

		Core::json base = Core::json::parse(data2D);
		Core::json src = Core::json::parse(data3D);

		base.at(Core::json::json_pointer(AtomPosAddr))[1] = src.at(Core::json::json_pointer(AtomPosAddr))[0];


		//std::cout << base.dump();
		return base.dump();
	}


}
