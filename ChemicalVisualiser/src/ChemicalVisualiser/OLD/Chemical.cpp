#include "Chemical.h"
#include <iostream>
//#include "raymath.h"
#include <Slate/Core.h>
#include "Layers/AppLayer.h"

namespace ChemVis 
{
	std::vector<std::string> Chemical::s_ElementSymbols = {
		"H",
		"He",
		"Li",
		"Be",
		"B",
		"C",
		"N",
		"O",
		"F",
		"Ne",
		"Na",
		"Mg",
		"Al",
		"Si",
		"P",
		"S",
		"Cl",
		"Ar",
		"K",
		"Ca",
		"Sc",
		"Ti",
		"V",
		"Cr",
		"Mn",
		"Fe",
		"Co",
		"Ni",
		"Cu",
		"Zn",
		"Ga",
		"Ge",
		"As",
		"Se",
		"Br",
		"Kr",
		"Rb",
		"Sr",
		"Y",
		"Zr",
		"Nb",
		"Mo",
		"Tc",
		"Ru",
		"Rh",
		"Pd",
		"Ag",
		"Cd",
		"In",
		"Sn",
		"Sb",
		"Te",
		"I",
		"Xe",
		"Cs",
		"Ba",
		"La",
		"Ce",
		"Pr",
		"Nd",
		"Pm",
		"Sm",
		"Eu",
		"Gd",
		"Tb",
		"Dy",
		"Ho",
		"Er",
		"Tm",
		"Yb",
		"Lu",
		"Hf",
		"Ta",
		"W",
		"Re",
		"Os",
		"Ir",
		"Pt",
		"Au",
		"Hg",
		"Tl",
		"Pb",
		"Bi",
		"Po",
		"At",
		"Rn",
		"Fr",
		"Ra",
		"Ac",
		"Th",
		"Pa",
		"U",
		"Np",
		"Pu",
		"Am",
		"Cm",
		"Bk",
		"Cf",
		"Es",
		"Fm",
		"Md",
		"No",
		"Lr",
		"Rf",
		"Db",
		"Sg",
		"Bh",
		"Hs",
		"Mt",
		"Ds",
		"Rg",
		"Cn",
		"Nh",
		"Fl",
		"Mc",
		"Lv",
		"Ts",
		"Og",
	};

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
			Slate::json DataJson = Slate::json::parse(data);
			return Chemical(ParseAtoms(DataJson), ParseBonds(DataJson), ParseInfo(DataJson));
		}
		catch (Slate::json::parse_error) {
			return std::optional<Chemical>();
		}
		catch (const std::exception& e)
		{
			//std::cout << "\n\n" << data << "\n\n";
			std::cerr << "ERROR In Chemical Parser: " << e.what() << "\n";
		}
	}

	AtomsInfo Chemical::ParseAtoms(Slate::json data)
	{
		const std::string AtomTypeAddr = "/PC_Compounds/0/atoms";
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0";

		Slate::json AtomTypeJson = data.at(Slate::json::json_pointer(AtomTypeAddr));
		Slate::json AtomPosJson = data.at(Slate::json::json_pointer(AtomPosAddr));
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

	BondsInfo Chemical::ParseBonds(Slate::json data)
	{
		const std::string BondsAddr = "/PC_Compounds/0/bonds";

		if (!data["PC_Compounds"][0].contains("bonds")) return BondsInfo{};
		Slate::json bondsJson = data.at(Slate::json::json_pointer(BondsAddr));

		BondsInfo bonds = {};
		bonds.BeginAtomIndices = bondsJson["aid1"].get<std::vector<int>>();
		bonds.EndAtomIndices = bondsJson["aid2"].get<std::vector<int>>();
		bonds.BondOrders = bondsJson["order"].get<std::vector<int>>();

		return bonds;
	}

	ChemicalInfo Chemical::ParseInfo(Slate::json data)
	{
		// Maybe use a seperate request for this information to be more accurate/informative
		ChemicalInfo chemicalInfo = {};
		
		chemicalInfo.Cid = std::to_string(data.at(Slate::json::json_pointer("/PC_Compounds/0/id/id")).value("cid", -1));

		const std::string InfoAddr = "/PC_Compounds/0/props";
		Slate::json InfoList = data.at(Slate::json::json_pointer(InfoAddr));
		if (!InfoList.is_array()) return chemicalInfo; 

		// Loop through all the chemical information
		// storing any that is wanted
		for (Slate::json info : InfoList)
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

	Slate::Color Chemical::GetAtomColor(int type)
	{
		auto elementColours = Slate::Application::Get().GetLayer<AppLayer>()->GetSettings().Values().ElementColors;
		const Slate::Color ERROR_COLOR = { 0, 0, 0, 255 };
		if (type < 1) return ERROR_COLOR;
		if (type > elementColours.size()) return ERROR_COLOR;
		unsigned int index = (type - 1) * 3;
		return Slate::Color{
			elementColours[index],
			elementColours[index + 1],
			elementColours[index + 2],
			255
		};
	}

	void Chemical::SetElementSymbols(std::vector<std::string> symbols)
	{
		s_ElementSymbols = symbols;
	}

	//TODO Replace with an API call to get the periodic table with all of its data (it can then be stored)
	std::string Chemical::GetAtomSymbol(int type) {
		if (type < 1) return "";
		if (type > s_ElementSymbols.size()) return "";
		return s_ElementSymbols[type - 1];
	}

	std::string Merge2Dand3D(std::string data2D, std::string data3D)
	{
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0/conformers";

		try
		{
			Slate::json base = Slate::json::parse(data2D);
			Slate::json src = Slate::json::parse(data3D);

			// VALIDATE
			if (base.contains("Fault")) {
				std::cerr << "ERROR: Fault in PubChem request: " << base["Fault"]["Message"] << "\n";
				return "";
			}
			else if (src.contains("Fault"))
			{
				return base.dump();
			}

			if (!base.contains("PC_Compounds"))
			{
				std::cerr << "ERROR: PubChem Response is missing data or in incorrect format\n";
				return "";
			}

			base.at(Slate::json::json_pointer(AtomPosAddr))[1] = src.at(Slate::json::json_pointer(AtomPosAddr))[0];

			return base.dump();
		}
		catch (Slate::json::parse_error)
		{
			//std::cout << data2D << "\n\n" << data3D << "\n\n\n";
			std::cerr << "ERROR: Could not Parse data returned from PubChem\n";
			return "";
		}
		catch (const std::exception& e)
		{
			std::cerr << "ERROR In Data Merger: " << e.what() << "\n";
			return "";
		}
	}
}
