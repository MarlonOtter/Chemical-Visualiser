#include "ChemicalSource.h"

namespace ChemicalVisualiser
{
	ChemicalNameSource::ChemicalNameSource(std::string Name, ChemicalList& List)
		: m_ChemicalName(Name), m_List(&List)
	{

	}

	std::vector<std::byte> ChemicalNameSource::Bytes() const
	{
		// check the map if it contains the chemical
		// if it does get from disk
		// otherwise get from API
		// return data

		//std::string data;
		//return std::vector<std::byte>(data.begin(), data.end());
		return std::vector<std::byte>();
	}
}