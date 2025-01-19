#pragma once

#include "GlobalClass.h"
#include <iostream>

struct Transforms
{
	std::vector<glm::vec3> pos;
	std::vector<glm::quat> rotat;
	std::vector<glm::vec3> scale;
};

struct AtomDrawData
{
	Transforms transforms;
	std::vector<glm::vec3> colours;
};


class DrawChemical
{
public:
	static void Draw(std::vector<Chemical> chemicals, Model& atomModel, Model& bondModel, Camera camera, Shader& atomShader, Shader& bondShader);

private:
	static AtomDrawData calcAtoms(Chemical& chemical);
	static Transforms calcBonds(Chemical& chemical);

	static int merge(AtomDrawData& src, AtomDrawData& dst);
	static int merge(Transforms& src, Transforms& dst);
	static int merge(std::vector<glm::vec3>& src, std::vector<glm::vec3>& dst);
};