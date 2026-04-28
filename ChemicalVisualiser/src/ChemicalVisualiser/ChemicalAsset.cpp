#include "ChemicalAsset.h"

namespace ChemicalVisualiser
{
	bool ChemicalAsset::Load(std::unique_ptr<Slate::AssetSource>& Source)
	{
		auto bytes = Source->Bytes();
		if (bytes.empty()) return false;

		std::string DataString = std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
		if (DataString.empty()) return false; 

		Chemical = CreateChemicalFromJson(DataString);
		return true;
	}
}