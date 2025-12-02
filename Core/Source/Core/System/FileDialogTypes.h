#pragma once
#include <string>
#include <vector>

namespace Core::System
{
	struct FileFilter
	{
		std::string Name;
		// List of all filetypes that are filtered e.g. png, jpg
		std::vector<std::string> Spec;
	};

	using FileFilterList = std::vector<FileFilter>;
}