#pragma once

#include "../Renderer/ObjectArray.h"

class Atom
{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		std::vector<Atom> bonds;
		
		int element;

		glm::vec3 colour;

		Atom(glm::vec3 pos, int element, std::vector<Atom> bonds);
		Atom(glm::vec3 pos, int element);
		void AddBond(Atom& atom);
		void Draw(Camera& camera);
		void Delete();	
};

