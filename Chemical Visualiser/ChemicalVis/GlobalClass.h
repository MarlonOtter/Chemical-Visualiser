#pragma once

#include "Chemical.h"

class globalClass
{
public:
	static Camera* camera;
	static std::vector<Chemical> chemicals;

	static float atomScale;

	static float bondRadius;
	static float bondLengthMultiplier;
	static float bondSeperationDist;

	static Shader* atomShader;
	static Model* atomModel;
};