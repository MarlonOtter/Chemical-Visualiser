#include "Atom.h"

Atom::Atom(int element, glm::vec3 pos3D, glm::vec2 pos2D)
{
	Atom::pos3D = pos3D;
	Atom::pos2D = pos2D;
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

void Atom::Delete()
{

}