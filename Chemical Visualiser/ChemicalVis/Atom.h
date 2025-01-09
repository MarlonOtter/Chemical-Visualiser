#pragma once

#include "../Renderer/Object.h"

class Atom
{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		std::vector<Atom> bonds;
		glm::vec3 colour;

		Object& object;

		Atom(glm::vec3 pos, glm::vec3 colour, std::vector<Atom> bonds, Object& object);
		void AddBond(Atom& atom);
		void Draw(Shader& shader, Camera& camera);
		void Delete();	
};

