#ifndef CHEMICAL_CLASS_H
#define CHEMICAL_CLASS_H

#include "Atom.h"


class Chemical
{
	public:
		const char* name;
		const char* formula;

		//Chemical(std::string name);
		Chemical(const char* atomicFormula);

		void MapFormula(std::string formula);

		void Draw(Shader& shader, Camera& camera);
};

#endif // !CHEMICAL_CLASS_H
