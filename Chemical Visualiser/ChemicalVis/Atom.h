#ifndef ATOM_CLASS_H
#define ATOM_CLASS_H

#include "../Renderer/Model.h"

class Atom
{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		int atomicNum;
		int maxBonds;
		std::vector<Atom> bonds;

		//Model model;

		Atom(int atomicNum, int maxBonds);
		void AddBond(Atom& atom);
		void Draw(Shader& shader, Camera& camera);
		void Delete();	
};
#endif // !ATOM_CLASS_H
