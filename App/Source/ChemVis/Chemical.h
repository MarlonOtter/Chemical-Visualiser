#pragma once

#include <string>
#include <vector>
#include <optional>

#include "raylib.h"
#include "Core/Renderer/Color.h"

#include "Core/Json.h"

namespace ChemVis {

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

		std::string dump()
		{
			std::stringstream out;
			out << "IUPAC: " << IupacName << ", "
				<< "Common: " << CommonName << ", "
				<< "Formula: " << MolecularFormula << ", "
				<< "Weight: " << MolecularWeight << ", "
				<< "SMILES: " << Smiles << ", "
				<< "InChI: " << InChI << ", "
				<< "CID: " << Cid;
			return out.str();
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
		std::vector<int> Types;
		Positions Positions2D;
		Positions Positions3D;
	};

	class Chemical
	{
	public:
		Chemical();
		Chemical(const AtomsInfo atoms, const BondsInfo bonds, const ChemicalInfo info);
		~Chemical();

		static std::optional<Chemical> Parse(std::string);
		
		const ChemicalInfo& GetInfo() const { return m_Info; }
		const AtomsInfo& GetAtoms() const { return m_Atoms; }
		const BondsInfo& GetBonds() const { return m_Bonds; }

		static Core::Color GetColor(int type);
	private:
		ChemicalInfo m_Info;
		AtomsInfo m_Atoms;
		BondsInfo m_Bonds;

		static AtomsInfo ParseAtoms(Core::json data);
		static BondsInfo ParseBonds(Core::json data);
		static ChemicalInfo ParseInfo(Core::json data);
	};

	std::string Merge2Dand3D(std::string data2D, std::string data3D);
}