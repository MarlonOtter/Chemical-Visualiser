#pragma once

#include <string>
#include <vector>

namespace ChemicalVisualiser
{
	std::string Get3D(std::string name);
	std::string Get2D(std::string name);
	std::string GetAutoComplete(std::string inp);
	std::vector<std::string> _ParseAutoComplete(std::string data);
}