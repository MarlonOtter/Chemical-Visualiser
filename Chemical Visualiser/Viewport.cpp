#include "Viewport.h"
#include <iostream>

Viewport::Viewport(int x, int y, int width, int height, bool lockWidth, bool lockHeight)
{
	pos = glm::vec2(x, y);
	size = glm::vec2(width, height);
	Viewport::lockWidth = lockWidth;
	Viewport::lockHeight = lockHeight;
}

Viewport::Viewport(glm::vec2 pos, glm::vec2 size, bool lockWidth, bool lockHeight)
{
	Viewport::pos = pos;
	Viewport::size = size;
	Viewport::lockWidth = lockWidth;
	Viewport::lockHeight = lockHeight;
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

void Viewport::AttachWindow(std::string name)
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoBackground;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	//need to find a way to detect when the user is resizeing a window reliably
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	ImGui::Begin(name.c_str(), 0, flags);
	hovered = ImGui::IsWindowHovered();
	focussed = ImGui::IsWindowFocused();

	ImVec2 windowSize = ImGui::GetWindowSize();
	//if axis is locked. keep the original value
	setSize(lockWidth ? size.x : windowSize.x, lockHeight ? size.y : windowSize.y);

	ImVec2 windowPos = ImGui::GetWindowPos();
	setPos(windowPos.x, windowPos.y);

	ImGui::End();
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
	setPos(glm::vec2(x, y));
}

void Viewport::setSize(glm::vec2 size)
{
	Viewport::size = size;
	if (size.x < 0) size.x = 0;
}

void Viewport::setSize(int w, int h)
{
	setSize(glm::vec2(w, h));
}

void Viewport::setSize(glm::vec2 relativeSize, glm::vec2 screenSize)
{
	setSize(glm::vec2((relativeSize.x * screenSize.x), screenSize.y));
}

bool Viewport::getHovered()
{
	return hovered;
}

bool Viewport::getFocussed()
{
	return focussed;
}

glm::vec2 Viewport::calculateRelativeSize(int screenWidth, int screenHeight)
{
	relativeSize = glm::vec2(glm::clamp(size.x / screenWidth, 0.0f, 1.0f), glm::clamp(size.y / screenHeight, 0.0f, 1.0f));
	return relativeSize;
}


