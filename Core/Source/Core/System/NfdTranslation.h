#pragma once

#include <vector>
#include <string>
#include <nfd.hpp>

#include "FileDialogTypes.h"

namespace Core::System
{
    // Translate between the NFD types and Types that the user uses
    std::vector<nfdfilteritem_t> ConvertFilters(const FileFilterList& Filters, std::vector<std::string>& Storage)
    {
        Storage.clear();
        Storage.reserve(Filters.size() * 2);

        std::vector<nfdfilteritem_t> NfdFilters;
        NfdFilters.reserve(Filters.size());

        for (size_t i = 0; i < Filters.size(); i++)
        {
            // Owning copies so that the data is not instantly dropped
            Storage.push_back(Filters[i].Name);
            Storage.push_back(ConvertSpec(Filters[i].Spec));

            nfdfilteritem_t item;
            item.name = Storage[Storage.size() - 2].c_str();
            item.spec = Storage.back().c_str();
            NfdFilters.push_back(item);
        }
        return NfdFilters;
    }

    std::string ConvertSpec(std::vector<std::string> Spec) {
        std::string result = "";
        for (int i = 0; i < Spec.size(); i++)
        {
            result += Spec[i] + ",";
        }
        return result.substr(0, result.size() - 1);
    };
}
