#include "PubChem.h"

#include <Slate/Http.h>
#include <Slate/Json.h>

#include <iostream>

namespace ChemVis::PubChem
{
	static std::string FormatStringForURL(const std::string& Input)
	{
		// TODO REPLACE all SPACES with %20
		return Input;
	}

	std::string Get3D(std::string name)
	{
		// std::string URL = "https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + Slate::String::Replace(name, ' ', "%20") + "/JSON";
		Slate::Http::Response Response = Slate::Http::Get({ 
			"https://pubchem.ncbi.nlm.nih.gov",
			"/rest/pug/compound/name/" + FormatStringForURL(name)+"/JSON?record_type=3d" 
			});
		return Response.Body();
	}

	std::string Get2D(std::string name)
	{
		Slate::Http::Response Response = Slate::Http::Get({
			"https://pubchem.ncbi.nlm.nih.gov",
			"/rest/pug/compound/name/" + FormatStringForURL(name)+"/JSON?record_type=2d"
			});
		return Response.Body();
	}

	std::string GetAutoComplete(std::string inp)
	{
		Slate::Http::Response Response = Slate::Http::Get({
			"https://pubchem.ncbi.nlm.nih.gov",
			"/rest/autocomplete/compound/" + FormatStringForURL(inp)+"/JSON?limit=5"
			});
		return Response.Body();
	}
	
	std::vector<std::string> _ParseAutoComplete(std::string data)
	{
		// TODO Check for parse error
		Slate::json jsonData = Slate::json::parse(data);
		if (jsonData.contains("Fault")) return {};

		std::vector<std::string> options;
		for (int i = 0; i < jsonData["total"]; i++)
		{
			std::string item = jsonData["dictionary_terms"]["compound"][i];
			options.push_back(item);
		}
		return options;
	}
}
