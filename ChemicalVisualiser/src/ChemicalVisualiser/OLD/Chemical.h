#pragma once

#include <string>
#include <vector>
#include <optional>
#include <format>

#include "Slate/Json.h"


namespace ChemicalVisualiser {

	struct Positions {
		std::vector<float> x;
		std::vector<float> y;
		std::vector<float> z;
	};

	struct ChemicalInfo
	{
		std::string IupacName = "";
		std::string CommonName = "";
		std::string MolecularFormula = "";
		std::string MolecularWeight = "";
		std::string Smiles = "";
		std::string InChI = "";
		std::string Cid = "";

		std::string Dump()
		{
			return std::format("IUPAC: {}, Common: {}, Formula: {}, Weight: {}, SMILES: {}, InChI: {}, CID: {}", 
				IupacName, CommonName, MolecularFormula, MolecularWeight, Smiles, InChI, Cid);
		}
	};

	struct BondsInfo
	{
		std::vector<int> BeginAtomIndices;
		std::vector<int> EndAtomIndices;
		std::vector<int> BondOrders;
	};

	struct AtomsInfo
	{
		Positions Positions2D;
		Positions Positions3D;
		std::vector<int> Types;
	};

	class Chemical
	{
	public:
		Chemical();
		Chemical(const AtomsInfo atoms, const BondsInfo bonds, const ChemicalInfo info);
		~Chemical();

		// Atom colour handling
		static glm::vec4 GetAtomColor(int type);

		// Atom Sybmol handling
		static std::string GetAtomSymbol(int type);
		static void SetElementSymbols(std::vector<std::string> symbols);

		static std::optional<Chemical> Parse(std::string);
		
		const ChemicalInfo& GetInfo() const { return m_Info; }
		const AtomsInfo& GetAtoms() const { return m_Atoms; }
		const BondsInfo& GetBonds() const { return m_Bonds; }

		
	private:
		ChemicalInfo m_Info;
		AtomsInfo m_Atoms;
		BondsInfo m_Bonds;

		static AtomsInfo ParseAtoms(Slate::json data);
		static BondsInfo ParseBonds(Slate::json data);
		static ChemicalInfo ParseInfo(Slate::json data);

		static std::vector<std::string> s_ElementSymbols;
	};

	std::string Merge2Dand3D(std::string data2D, std::string data3D);
}