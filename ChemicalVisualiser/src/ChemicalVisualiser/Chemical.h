#pragma once

#include <string>
#include <vector>
#include <Slate/Math.h>

namespace ChemicalVisualiser
{
	struct Atoms
	{
		std::vector<uint8_t> Type;
		std::vector<glm::vec2> Position2D;
		std::vector<glm::vec3> Position3D;
		size_t Count = 0;
	};

	struct Bonds 
	{
		std::vector<size_t> AtomA;
		std::vector<size_t> AtomB;
		std::vector<uint8_t> Order;
		size_t Count = 0;
	};

	struct ChemicalInformation
	{
		std::string IUPACName;
		std::string CommonName;
		std::string Formula;
		std::string SMILES;
		std::string InChI;
		uint64_t CID = 0;
		double Mass = 0.0;
	};

	struct Chemical
	{
		Atoms Atoms;
		Bonds Bonds;
		ChemicalInformation ChemicalInformation;
	};

	Chemical CreateChemicalFromJson(const std::string& JsonString);
	std::string GetElementSymbol(uint8_t AtomicNumber);
}