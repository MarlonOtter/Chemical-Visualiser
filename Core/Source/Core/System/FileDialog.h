#pragma once
#include <string>
#include <vector>

#include "FileDialogTypes.h"

namespace Core::System
{
	std::string SaveDialog(const FileFilterList& Filters, const std::string defaultPath = "", const std::string defaultFilename = "untitled");
	std::string OpenDialog(const FileFilterList& filters, const std::string defaultPath);
	std::string PickFolderDialog();
}