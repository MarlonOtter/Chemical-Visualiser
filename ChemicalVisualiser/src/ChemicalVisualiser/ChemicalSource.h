#pragma once

#include <string>
#include <memory>
#include <Slate/Assets.h>

#include "ChemicalList.h"

namespace ChemicalVisualiser
{
	class ChemicalNameSource : public Slate::AssetSource
	{
	public:
		ChemicalNameSource(std::string Name, ChemicalList& List);

		std::vector<std::byte> Bytes() const override;
		std::string Identifier() const override { return m_ChemicalName; }

		Slate::AssetMetaData MetaData() const override { return {}; };
		bool CanHotReload() const override { return false; }
	
	private:
		std::string m_ChemicalName;
		ChemicalList* m_List;
	};
}