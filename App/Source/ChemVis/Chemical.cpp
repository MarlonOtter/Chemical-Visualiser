#include "Chemical.h"
#include <iostream>
#include "raymath.h"


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

	std::optional<Chemical> Chemical::Parse(std::string data)
	{
		try
		{
			// Try and parse the JSON data
			Core::json DataJson = Core::json::parse(data);
			return Chemical(ParseAtoms(DataJson), ParseBonds(DataJson), ParseInfo(DataJson));
		}
		catch (Core::json::parse_error) {
			// If not valid JSON data
			return std::optional<Chemical>();
		}
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

	Core::Color Chemical::GetColor(int type)
	{
		switch (type)
		{
			case 1: {
				return Core::WHITE;
			}
			case 6: {
				return Core::GRAY;
			}
			case 7: {
				return Core::BLUE;
			}
			case 8: {
				return Core::RED;
			}
			default: {
				return Core::PINK;
			}
		}
	}

	std::string Merge2Dand3D(std::string data2D, std::string data3D)
	{
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0/conformers";

		try
		{
			Core::json base = Core::json::parse(data2D);
			Core::json src = Core::json::parse(data3D);

			// VALIDATE
			if (base.contains("Fault")) {
				std::cout << "ERROR: Fault in PubChem request: " << base["Fault"]["Message"] << "\n";
				return "";
			}
			else if (src.contains("Fault"))
			{
				return base.dump();
			}

			if (!base.contains("PC_Compounds"))
			{
				std::cout << "ERROR: PubChem Response is missing data or in incorrect format\n";
				return "";
			}

			base.at(Core::json::json_pointer(AtomPosAddr))[1] = src.at(Core::json::json_pointer(AtomPosAddr))[0];

			return base.dump();
		}
		catch (Core::json::parse_error)
		{
			std::cout << "ERROR: Could not Parse data returned from PubChem";
			return "";
		}
	}


}
