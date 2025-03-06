#pragma once

#include "GlobalClass.h"
#include <iostream>

struct Transforms
{
	std::vector<glm::vec3> pos;
	std::vector<glm::quat> rotat;
	std::vector<glm::vec3> scale;

	void clear()
	{
		pos.clear();
		rotat.clear();
		scale.clear();
	}
};

struct AtomDrawData
{
	Transforms transforms;
	std::vector<glm::vec3> colours;

	void clear()
	{
		transforms.clear();
		colours.clear();
	}
};

struct drawData
{
	std::vector<glm::mat4> atom3DMat4s;
	std::vector<glm::mat4> atom2DMat4s;
	std::vector<glm::mat4> bond2DMat4s;
	std::vector<glm::mat4> bond3DMat4s;

	bool done3D = false;
	bool done2D = false;

	std::vector<glm::mat4>& operator[](int index)
	{
		switch (index)
		{
		case 0:
			return atom2DMat4s;
		case 1:
			return bond2DMat4s;
		case 2:
			return atom3DMat4s;
		case 3:
			return bond3DMat4s;
		default:
			throw std::out_of_range("Invalid index for drawData");
		}
	}

	void clear()
	{
		atom3DMat4s.clear();
		atom2DMat4s.clear();
		bond2DMat4s.clear();
		bond3DMat4s.clear();

		done3D = false;
		done2D = false;
	}
};


class DrawChemical
{
public:
	static void Draw(std::vector<Chemical> chemicals, Model& atomModel, Model& bondModel, Camera camera, Shader& atomShader, Shader& bondShader, int format = 1);

	static glm::vec3 rotateAround(glm::vec3 pos, float degrees, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));
private:
	static int prevChemicalCount;

	static drawData chemicalDrawData;
	static std::vector<glm::vec3> atomColours;

	static AtomDrawData calcAtoms(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset);
	static Transforms calcBonds(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset);

	static int merge(AtomDrawData& src, AtomDrawData& dst);
	static int merge(Transforms& src, Transforms& dst);
	static int merge(std::vector<glm::vec3>& src, std::vector<glm::vec3>& dst);
};