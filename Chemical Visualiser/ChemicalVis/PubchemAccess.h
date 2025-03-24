#pragma once

//library that allows for http requests
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "../Renderer/ShaderClass.h"

struct throttleValues
{
	float count;
	float time;
	float service;
};

class PubChem
{
public:
	//INFO: chemical structure is in the SMILES format 
	//		(e.g CCC = propane)

	static throttleValues throttle;

	//data input methods 
	static std::string name(std::string name);
	static std::string structure(std::string struc);
	static std::string cid(int cid);

	//get the conformers data (2D / 3D structure)
	static std::string conformers_name(std::string name);
	static std::string conformers_struc(std::string struc);
	static std::string conformers_cid(int cid);

	//input a chemical name and it will provide chemicals with similar names
	static std::string autoComplete(std::string str, int out = 5);

private:
	//makes the request to the server
	static std::string request(std::string url);
};



/*
	Issue:
		HTTPLIB is a blocking I/O. This means that the main thread freezes while it is waiting for data to be
		returned and it cannot be done on anouther thread. This means that the program freezes whenever a
		request is made. I don't know if this is what I want or not.

		Actually this might be a big issue if i want to include autocomplete as it will cause the program to
		freeze while typeing.. very inconvinient. I could solve this by either no auto-complete or a new library that
		uses non-blocking socket I/O
		Problems solved -> this was a misinterptretation of what "blocking" actually means... it measn that only one request
		can be made at a time... this is all i need to be able to do. I just have to put it on anouther thread to not freeze
		the entire program
*/