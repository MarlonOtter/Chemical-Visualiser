
//impport the header
#include "PubchemAccess.h"


throttleValues PubChem::throttle = { 0,0,0 };

// PUBLIC METHODS

// search the database with its name
std::string PubChem::name(std::string name)
{
	return request("/rest/pug/compound/name/" + name + "/JSON");
};

// search the database with structure
std::string PubChem::structure(std::string struc)
{
	return request("/rest/pug/compound/smiles/" + struc + "/JSON");
};

// search the database with its CID
std::string PubChem::cid(int cid)
{
	return request("/rest/pug/compound/cid/" + std::to_string(cid) + "/JSON");
}


//Get conformer data from the chemicals name
std::string PubChem::conformers_name(std::string name)
{
	return request("/rest/pug/compound/name/" + name + "/JSON?record_type=3d");
}

// Get the chemicals conformer data from its structure
std::string PubChem::conformers_struc(std::string struc)
{
	return request("/rest/pug/compound/smiles/" + struc + "/JSON?record_type=3d");
}

// Get the chemicals conformer data from its cid 
std::string PubChem::conformers_cid(int cid)
{
	return request("/rest/pug/compound/cid/" + std::to_string(cid) + "/JSON?record_type=3d");
}

// search chemicals with a similar name as the provided string
std::string PubChem::autoComplete(std::string str, int out)
{
	if (throttle.count >= 75.0 || throttle.service >= 75.0) return "";

	//if (str == "") return get_file_contents();

	// will output "out" many chemicals
	return request("/rest/autocomplete/compound/" + str + "/JSON?limit=" + std::to_string(out));
};




// PRIVATE METHODS

//requests the data from the provided page
std::string PubChem::request(std::string url) 
{
	//return get_file_contents("ChemicalData/65157.json");;
	std::cout << "Making Request: " << url << "\n";
	// creates a client with the server
	httplib::SSLClient cli("pubchem.ncbi.nlm.nih.gov");

	// enable certificate for https
	cli.enable_server_certificate_verification(true);
	//follow any page redirects
	cli.set_follow_location(true); 

	
	//get the data from the desired page
	auto res = cli.Get(url);
	
	//check that a connection is made
	if (res) { 
		// output any status errors
		if (res->status != 200) { 
			// Don't output it, if the error is just a user input error
			if (res->status != 400 && res->status != 404)
			{
				std::cout << "Status: " << res->status << "\n";
			}
			
		}

		// Loop through all the headers
		for (const auto& header : res->headers) {
			// If the header is not the one being searched for, move onto the next one
			if (header.first != "X-Throttling-Control") continue;
			
			int offset = 0;
			int i = 0;
			// Loop until the offset is bigger than the size of the string
			while (offset < header.second.size())
			{
				// Get the location of the open and close brackets
				size_t openLoc = header.second.find("(", offset);
				size_t closeLoc = header.second.find(")", offset);

				// If either of the brackets don't exist, stop looping
				if (openLoc == std::string::npos || closeLoc == std::string::npos) break;

				// Get the string that contains the numbers
				std::string data = header.second.substr(openLoc+1, closeLoc - (openLoc+1) - 1);

				// Set the relevent value in the throttle struct
				if (i == 0) throttle.count = std::stof(data);
				else if (i == 1) throttle.time = std::stof(data);
				else if (i == 2) throttle.service = std::stof(data);
				else std::cout << "PubChemAccess.cpp, ERROR : Throttle Header contains more data than expected\n";
				
				// Increment i and offset
				++i;
				offset = closeLoc + 1;
			}
			std::cout << "Throttle: " << throttle.count << ", " << throttle.time << ", " << throttle.service << "\n";

			break;
		}
		//return page body as a string
		return res->body; 
	}
	else {	
		//if there is an error output the error to terminal and return nothing
		std::cerr << "Error: " << httplib::to_string(res.error()) << "\n";
	}
	return "";
}