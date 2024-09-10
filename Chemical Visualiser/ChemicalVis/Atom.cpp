
#include "Atom.h"

Atom::Atom(int atomicNum, int maxBonds)
{
	Atom::atomicNum = atomicNum;
	Atom::maxBonds = maxBonds;

	model = Model("Resources/Models/Spheres/1xV2.obj", pos);
}

void Atom::Draw(Shader& shader, Camera& camera)
{
	model.Draw(shader, camera);
}

void Atom::AddBond(Atom& atom)
{
	//add new atom to bond vector

	//add bond mesh / change to correct mesh
}

void Atom::Delete()
{
	//model.Delete();
}