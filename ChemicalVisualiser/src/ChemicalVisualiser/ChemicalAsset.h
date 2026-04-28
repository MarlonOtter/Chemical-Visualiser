#pragma once

#include <string>

#include <Slate/Assets.h>

#include "Chemical.h"

namespace ChemicalVisualiser
{
	class ChemicalAsset : public Slate::Asset
	{
	public:
		bool Load(std::unique_ptr<Slate::AssetSource>& Source) override;

		Chemical Chemical;

		SLATE_ASSET_TYPE(ChemicalAsset)
	};
}