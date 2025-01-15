#include "Atom.h"

Atom::Atom(glm::vec3 pos, int element, std::vector<Atom> bonds)
{
	Atom::pos = pos;
	Atom::element = element;
}

Atom::Atom(glm::vec3 pos, int element)
{
	Atom::pos = pos;
	Atom::element = element;

	switch (element)
	{
	case 0: // Nothing
		break;
	case 6: // Carbon
		colour = glm::vec3(0.5f, 0.5f, 0.5f);
		break;
	case 1: // Hydrogen 
		colour = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case 8: // Oxygen
		colour = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case 7: // Nitrogen
		colour = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

void Atom::AddBond(Atom& atom)
{

}

void Atom::Delete()
{

}