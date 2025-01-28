#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"

class Viewport
{
public:

	Viewport(int x, int y, int width, int height);
	Viewport(glm::vec2 pos, glm::vec2 size);

	void Update(glm::vec3 colour);
	void Update(float colour[3]);

	glm::vec2 getPos();
	glm::vec2 getSize();

	void setPos(glm::vec2 pos);
	void setPos(int x, int y);

	void setSize(glm::vec2 size);
	void setSize(int x, int y);

	

private:
	glm::vec2 pos;
	glm::vec2 size;
};
