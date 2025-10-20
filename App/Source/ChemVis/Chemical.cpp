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
			return std::optional<Chemical>();
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR In Chemical Parser: " << e.what() << "\n";
		}
	}

	AtomsInfo Chemical::ParseAtoms(Core::json data)
	{
		const std::string AtomTypeAddr = "/PC_Compounds/0/atoms";
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0";

		Core::json AtomTypeJson = data.at(Core::json::json_pointer(AtomTypeAddr));
		Core::json AtomPosJson = data.at(Core::json::json_pointer(AtomPosAddr));
		size_t AtomCount = AtomPosJson["aid"].size();
		
		AtomsInfo atoms = {};
		atoms.Types = AtomTypeJson["element"].get<std::vector<int>>();

		Positions pos0 = {
				AtomPosJson["conformers"][0]["x"],
				AtomPosJson["conformers"][0]["y"],
				std::vector<float>(AtomCount, 0.0f),
		};
		if (AtomPosJson["conformers"][0].contains("z"))
		{
			pos0.z = AtomPosJson["conformers"][0]["z"].get<std::vector<float>>();
			atoms.Positions3D = pos0;
		}
		else atoms.Positions2D = pos0;

		if (AtomPosJson["conformers"].size() > 1)
		{
			Positions pos1 = {
					AtomPosJson["conformers"][1]["x"],
					AtomPosJson["conformers"][1]["y"],
					std::vector<float>(AtomCount, 0.0f),
			};
			if (AtomPosJson["conformers"][1].contains("z"))
			{
				pos1.z = AtomPosJson["conformers"][1]["z"].get<std::vector<float>>();
				atoms.Positions3D = pos1;
			}
			else atoms.Positions2D = pos1;
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
		// Maybe use a seperate request for this information to be more accurate/informative
		ChemicalInfo chemicalInfo = {};
		
		chemicalInfo.Cid = std::to_string(data.at(Core::json::json_pointer("/PC_Compounds/0/id/id")).value("cid", -1));

		const std::string InfoAddr = "/PC_Compounds/0/props";
		Core::json InfoList = data.at(Core::json::json_pointer(InfoAddr));
		if (!InfoList.is_array()) return chemicalInfo; 

		// Loop through all the chemical information
		// storing any that is wanted
		for (Core::json info : InfoList)
		{
			std::string label = info["urn"].value("label", "");
			if (label.empty()) continue;
			std::string value = info["value"].value("sval", "N/a");
	
			if (label == "IUPAC Name")
			{
				std::string name = info["urn"]["name"];
				if (name == "Systematic")
				{
					chemicalInfo.IupacName = value;
				}
				else if (name == "Traditional")
				{
					chemicalInfo.CommonName = value;
				}
			}
			else if (label == "InChI")
			{
				chemicalInfo.InChI = value;
			}
			else if (label == "Molecular Formula")
			{
				chemicalInfo.MolecularFormula = value;
			}
			else if (label == "Molecular Weight")
			{
				chemicalInfo.MolecularWeight = value;
			}
			else if (label == "SMILES")
			{
				std::string name = info["urn"]["name"];
				if (name == "Absolute")
				{
					chemicalInfo.Smiles = value;
				}
			}
		}

		return chemicalInfo;
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
		catch (const std::exception& e)
		{
			std::cout << "ERROR In Data Merger: " << e.what() << "\n";
			return "";
		}
	}
}
