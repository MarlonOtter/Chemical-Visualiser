#include "PubChem.h"

#include "Core/Http/HttpClient.h"
#include "Core/Utils/String.h"

#include <iostream>

namespace ChemVis::PubChem
{
	std::string Get3D(std::string name)
	{
		std::string URL = "https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + Core::String::Replace(name, ' ', "%20") + "/JSON";
		Core::Http::HttpResponse Response = Core::Http::Client::Get(URL + "?record_type=3d");
		return Response.body;
	}

	std::string Get2D(std::string name)
	{
		std::string URL = "https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + Core::String::Replace(name, ' ', "%20") + "/JSON";
		Core::Http::HttpResponse Response = Core::Http::Client::Get(URL + "?record_type=2d");
		return Response.body;
	}

	std::string GetAutoComplete(std::string inp)
	{
		std::string URL = "https://pubchem.ncbi.nlm.nih.gov/rest/autocomplete/compound/" + Core::String::Replace(inp, ' ', "%20") + "/JSON?limit=" + std::to_string(5);
		Core::Http::HttpResponse Response = Core::Http::Client::Get(URL);
		return Response.body;
	}
	
	std::vector<std::string> _ParseAutoComplete(std::string data)
	{
		// should check for parse error
		Core::json jsonData = Core::json::parse(data);
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
