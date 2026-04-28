#include "Chemical.h"

#include <Slate/Json.h>

namespace ChemicalVisualiser
{
	static Atoms ParseAtoms(const Slate::json& Json)
	{
		Atoms Atoms;
		
		const std::string AtomTypeAddr = "/PC_Compounds/0/atoms";
		const std::string AtomPosAddr = "/PC_Compounds/0/coords/0";

		const Slate::json& AtomTypeJson = Json.at(Slate::json::json_pointer(AtomTypeAddr));
		const Slate::json& AtomPosJson = Json.at(Slate::json::json_pointer(AtomPosAddr));
		Atoms.Count = AtomPosJson["aid"].size();

		Atoms.Type = AtomTypeJson["element"].get<std::vector<uint8_t>>();	

		Atoms.Position2D.reserve(Atoms.Count);
		std::vector<float> x = AtomPosJson["conformers"][0]["x"].get<std::vector<float>>();
		std::vector<float> y = AtomPosJson["conformers"][0]["y"].get<std::vector<float>>();
		
		for (size_t i = 0; i < Atoms.Count; i++)
		{
			Atoms.Position2D.push_back({ x[i], y[i] });
		}

		// If Contains 3D Data
		if (AtomPosJson["conformers"].size() == 2) 
		{
			Atoms.Position3D.reserve(Atoms.Count);
			std::vector<float> x = AtomPosJson["conformers"][1]["x"].get<std::vector<float>>();
			std::vector<float> y = AtomPosJson["conformers"][1]["y"].get<std::vector<float>>();
			std::vector<float> z = AtomPosJson["conformers"][1]["z"].get<std::vector<float>>();

			for (size_t i = 0; i < Atoms.Count; i++)
			{
				Atoms.Position3D.push_back({ x[i], y[i], z[i] });
			}
		}

		return Atoms;
	}

	static Bonds ParseBonds(const Slate::json& Json)
	{
		const std::string BondsAddr = "/PC_Compounds/0/bonds";

		if (!Json["PC_Compounds"][0].contains("bonds")) return {};
		const Slate::json& bondsJson = Json.at(Slate::json::json_pointer(BondsAddr));

		return Bonds{
			.AtomA = bondsJson["aid1"].get<std::vector<size_t>>(),
			.AtomB = bondsJson["aid2"].get<std::vector<size_t>>(),
			.Order = bondsJson["order"].get<std::vector<uint8_t>>(),
			.Count = bondsJson["aid1"].size(),
		};
	}

	static ChemicalInformation ParseInformation(const Slate::json& Json)
	{
		ChemicalInformation Information;
		Information.CID = Json.at(Slate::json::json_pointer("/PC_Compounds/0/id/id")).value("cid", 0);

		const std::string InfoAddr = "/PC_Compounds/0/props";
		const Slate::json& InfoList = Json.at(Slate::json::json_pointer(InfoAddr));
		
		if (!InfoList.is_array()) return Information;

		for (Slate::json info : InfoList)
		{
			std::string label = info["urn"].value("label", "");
			if (label.empty()) continue;
			const Slate::json& ValuePtr = info["value"];

			if (label == "IUPAC Name")
			{
				std::string name = info["urn"].value("name", "");
				if (name == "Systematic")
				{
					Information.IUPACName = ValuePtr.value("sval", "");
				}
				else if (name == "Traditional")
				{
					// this is unreliable as it isn't always it's Common Name 
					Information.CommonName = ValuePtr.value("sval", "");
				}
			}
			else if (label == "InChI")
			{
				Information.InChI = ValuePtr.value("sval", "");
			}
			else if (label == "Molecular Formula")
			{
				Information.Formula = ValuePtr.value("sval", "");
			}
			else if (label == "Molecular Weight")
			{
				Information.Mass = ValuePtr.value("sval", 0.0);
			}
			else if (label == "SMILES")
			{
				std::string name = info["urn"]["name"];
				if (name == "Absolute")
				{
					Information.SMILES = ValuePtr.value("sval", "");
				}
			}
		}

		return Information;
	}

	Chemical CreateChemicalFromJson(const std::string& JsonString)
	{
		// TODO Validate the JsonString first.
		Slate::json Json = Slate::json::parse(JsonString);
		return Chemical{ 
			ParseAtoms(Json),
			ParseBonds(Json),
			ParseInformation(Json) 
		};
	}
	std::string GetElementSymbol(uint8_t AtomicNumber)
	{
		return "*";
	}
}