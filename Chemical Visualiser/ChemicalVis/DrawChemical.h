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
	static void Draw(std::vector<Chemical> chemicals, Model& atomModel, Model& bondModel, Camera camera, Shader& atomShader, Shader& bondShader, int format = 1);

	static glm::vec3 rotateAround(glm::vec3 pos, float degrees, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));
private:
	static AtomDrawData calcAtoms(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset);
	static Transforms calcBonds(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset);

	static int merge(AtomDrawData& src, AtomDrawData& dst);
	static int merge(Transforms& src, Transforms& dst);
	static int merge(std::vector<glm::vec3>& src, std::vector<glm::vec3>& dst);
};