#pragma once

#include "Chemical.h"

class globalClass
{
public:
	static Camera* camera3D;
	static Camera* camera2D;
	static std::vector<Chemical> chemicals;

	static float atomScale;

	static float bondRadius;
	static float bondLengthMultiplier;
	static float bondSeperationDist;

	static float chemicalSeperationDist;

	static Shader* atomShader;
	static Model* atomModel;
};