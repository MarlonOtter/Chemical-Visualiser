#pragma once

#include <string>
#include "glm/glm.hpp"
#include "glad/glad.h"
#include "imgui/imgui.h"


class Viewport
{
public:
	Viewport();
	Viewport(int x, int y, int width, int height, bool lockWidth = false, bool lockHeight = false);
	Viewport(glm::vec2 pos, glm::vec2 size, bool lockWidth = false, bool lockHeight = false);

	void Update(glm::vec3 colour);

	void AttachWindow(std::string name);

	glm::vec2 getPos();
	glm::vec2 getSize();

	void setPos(glm::vec2 pos);
	void setPos(int x, int y);

	void setSize(glm::vec2 size);
	void setSize(int x, int y);
	void setSize(glm::vec2 relativeSize, glm::vec2 screenSize);

	bool getHovered();
	bool getFocussed();

	glm::vec2 calculateRelativeSize(int screenWidth, int screenHeight);

private:
	bool hovered = false;
	bool focussed = false;

	bool lockWidth;
	bool lockHeight;
	
	glm::vec2 relativeSize = glm::vec2(0.5, 1.0);
	glm::vec2 pos;
	glm::vec2 size;
};
