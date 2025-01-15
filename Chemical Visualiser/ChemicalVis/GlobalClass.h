#pragma once

#include "Chemical.h"

class globalClass
{
public:
	static Camera* camera;
	static std::vector<Chemical> chemicals;

	static Shader* atomShader;
	static Model* atomModel;
};