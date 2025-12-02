#include "FileDialog.h"

#include <iostream>
#include <nfd.hpp>

#include "NfdTranslation.h"

namespace Core::System
{
	std::string SaveDialog(const FileFilterList& Filters, const std::string defaultPath, const std::string defaultFilename)
	{
        NFD::Guard nfdGuard;
        NFD::UniquePath outPath;
        
        std::vector<std::string> FilterStorage;
        std::vector<nfdfilteritem_t> NfdFilters = ConvertFilters(Filters, FilterStorage);
        
        nfdresult_t result = NFD::SaveDialog(outPath,
            NfdFilters.empty() ? nullptr : NfdFilters.data(),
            static_cast<nfdfiltersize_t>(NfdFilters.size()),
            defaultPath.empty() ? nullptr : defaultPath.c_str(),
            defaultFilename.c_str()
        );
        
        if (result == NFD_OKAY) {
            std::cout << "Save Dialog Path : " << outPath.get() << "\n";
            return outPath.get();
        }
        else if (result == NFD_CANCEL) {
            std::cout << "Cancelled Save Dialog\n";
        }
        else {
            std::cout << "Save Dialog Error: " << NFD::GetError() << std::endl;
        }       
        return "";
	}

    
}