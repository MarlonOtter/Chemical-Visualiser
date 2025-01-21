#pragma once

#include "../Renderer/ObjectArray.h"

class Atom
{
	public:
		//position data in 2D and 3D
		glm::vec3 pos3D = glm::vec3(0);
		glm::vec2 pos2D = glm::vec2(0);
		
		//the element (nuclear charge)
		int element;

		//colour that the atom should be displayed in
		//this should probably be changed to just a lookup on a table with all the elements
		//to their colour
		glm::vec3 colour = glm::vec3(0);

		//constructor
		Atom(int element = 0, glm::vec3 pos3D = glm::vec3(0), glm::vec2 pos2D = glm::vec2(0));
		
		// don't think this is really needed but i have it anyway
		void Delete();	
};

