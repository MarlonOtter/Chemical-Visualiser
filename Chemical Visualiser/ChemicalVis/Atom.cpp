#include "Atom.h"

Atom::Atom(int element, glm::vec3 pos3D, glm::vec2 pos2D)
{
	Atom::pos3D = pos3D;
	Atom::pos2D = pos2D;
	Atom::element = element;

	// NEED TO MAKE THESE REFERENCE THE SETTINGS FILE SO THAT I
	// DON'T HAVE TO MANUALLY SET OUT EACH ONE INDIVIDUALLY

	switch (element)
	{
	case 0: // Nothing
		break;
	case 1: // Hydrogen 
		colour = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case 6: // Carbon
		colour = glm::vec3(0.5f, 0.5f, 0.5f);
		break;
	case 7: // Nitrogen
		colour = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case 8: // Oxygen
		colour = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case 16: // Sulfur
		colour = glm::vec3(0.8078, 0.6824, 0.0353);
		break;
	case 17: // Chlorine
		colour = glm::vec3(0.1216, 0.7843, 0.0980);
		break;
	case 35: // Bromine
		colour = glm::vec3(0.3216, 0.0980, 0.0314);
		break;
	case 53: // Iodine
		colour = glm::vec3(0.7216, 0.0667, 0.7922);
		break;
	default:
		break;
	}
}

void Atom::Delete()
{

}