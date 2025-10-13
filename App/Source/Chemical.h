#pragma once

#include <string>
#include <vector>

#include "raylib.h"

struct ChemicalInfo
{
	std::string name;
	std::string formula;
	float molecularWeight;
};

class Chemical 
{
public:
	Chemical();
	~Chemical();


	// read from json api
	static Chemical& Parse(std::string);

	ChemicalInfo GetInfo() const { return m_Info; }
	const std::vector<Vector3>& GetAtomPositions() const { return m_AtomPositions; }
	const std::vector<char>& GetAtomTypes() const { return m_AtomTypes; }

private:
	ChemicalInfo m_Info;

	std::vector<Vector3> m_AtomPositions;
	std::vector<char> m_AtomTypes;
};