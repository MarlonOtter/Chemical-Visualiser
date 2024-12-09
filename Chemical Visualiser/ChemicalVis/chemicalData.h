#ifndef CHEMICAL_DATA_H
#define CHEMICAL_DATA_H

struct Chemical
{
	char* name;
	char* structure;
	char* properties;
	Atom* atoms;

};

struct Atom
{
	char* type;
	float pos;
	//parent chemical reference
	Chemical* parentChemical;
};

class chemicalData
{
public:
	char* jsonData;

	void fetchData(char* name);
	char* parseJson(char* attribute);

};





#endif // !CHEMICAL_DATA_H
