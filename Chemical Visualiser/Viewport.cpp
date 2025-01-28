#include "Viewport.h"

Viewport::Viewport(int x, int y, int width, int height)
{
	pos = glm::vec2(x, y);
	size = glm::vec2(width, height);
}

Viewport::Viewport(glm::vec2 pos, glm::vec2 size)
{
	Viewport::pos = pos;
	Viewport::size = size;
}

void Viewport::Update(glm::vec3 colour)
{
	//create the viewport
	glViewport(pos.x, pos.y, size.x, size.y);

	//set the background colour
	glClearColor(colour.r, colour.g, colour.b, 1.0f);

	//split the two viewports into seperate render targets
	glScissor(pos.x, pos.y, size.x, size.y);
	glEnable(GL_SCISSOR_TEST);

	//clears the buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewport::Update(float colour[3])
{
	Update(glm::vec3(colour[0], colour[1], colour[2]));
}

glm::vec2 Viewport::getPos()
{
	return pos;
}

glm::vec2 Viewport::getSize()
{
	return size;
}

void Viewport::setPos(glm::vec2 pos)
{
	Viewport::pos = pos;
}

void Viewport::setPos(int x, int y)
{
	Viewport::pos = glm::vec2(x, y);
}

void Viewport::setSize(glm::vec2 size)
{
	Viewport::size = size;
}

void Viewport::setSize(int w, int h)
{
	Viewport::size = glm::vec2(w, h);
}





