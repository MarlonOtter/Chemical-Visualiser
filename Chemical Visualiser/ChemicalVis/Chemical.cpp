
#include "Chemical.h"


Chemical::Chemical(const char* atomicFormula)
{
	Chemical::formula = atomicFormula;
	MapFormula(formula);
}

void Chemical::MapFormula(std::string formula)
{
	// validate
	// maybe just check for invalid characters

	// loop through each character
	for (int i = 0; i < formula.size(); i++)
	{
		if (formula[i] == '/')
		{

		}
	}
}

void Chemical::Draw(Shader& shader, Camera& camera)
{

}