
//impport the header
#include "DataFetch.h"

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
	// get the conformeer ID
	std::string conformerID = request("/rest/pug/compound/name/"+ name +"/conformers/TXT");
	return conformer(conformerID); // return data
}

// Get the chemicals conformer data from its structure
std::string PubChem::conformers_struc(std::string struc)
{
	// get the conformeer ID
	std::string conformerID = request("/rest/pug/compound/smiles/" + struc + "/conformers/TXT");
	return conformer(conformerID); // return data
}

// Get the chemicals conformer data from its cid 
std::string PubChem::conformers_cid(int cid)
{
	// get the conformeer ID
	std::string conformerID = request("/rest/pug/compound/cid/" + std::to_string(cid) + "/conformers/TXT");
	return conformer(conformerID); // return data
}


// search chemicals with a similar name as the provided string
std::string PubChem::autoComplete(std::string str, int out)
{
	// will output "out" many chemicals
	return request("/rest/autocomplete/compound/" + str + "/JSON?limit=" + std::to_string(out));
};




// PRIVATE METHODS

//requests the data from the provided page
std::string PubChem::request(std::string url) 
{
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
		// output any status errors (e.g. 404 page not found)
		if (res->status != 200) { 
			std::cout << "Status: " << res->status << "\n"; 
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

// Get the conformer data from its conformer ID
std::string PubChem::conformer(std::string ID)
{
	// if there is an error in the ID
	if (ID.substr(0, 11) == "Status: 404")
	{
		return ""; // return nothing
	}
	// else get the atom position data of the chemical
	return request("/rest/pug/conformers/" + ID.substr(0,16) + "/JSON");
}