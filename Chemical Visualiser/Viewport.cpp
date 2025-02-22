#include "Viewport.h"
#include <iostream>
Viewport::Viewport()
{
	return;
}

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
	glViewport(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(size.x), static_cast<float>(size.y));

	//set the background colour
	glClearColor(colour.r, colour.g, colour.b, 1.0f);

	//split the two viewports into seperate render targets
	glScissor(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(size.x), static_cast<float>(size.y));
	glEnable(GL_SCISSOR_TEST);

	//clears the buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewport::AttachWindow(std::string name)
{
	// Flags that define how the window will be shown to the user
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoBackground;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoTitleBar;
	
	// Set the window size and position to the same as the viewport
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	// Create the window
	ImGui::Begin(name.c_str(), 0, flags);

	// Read if the user is using that window
	hovered = ImGui::IsWindowHovered();
	focussed = ImGui::IsWindowFocused();
	
	// get the window size if it has been resized by the user
	ImVec2 windowSize = ImGui::GetWindowSize();
	// If axis is locked. keep the original value
	setSize(static_cast<int>(lockWidth ? size.x : windowSize.x), static_cast<int>(lockHeight ? size.y : windowSize.y));

	// Get the new pos and set the viewport as the same position
	ImVec2 windowPos = ImGui::GetWindowPos();
	setPos(static_cast<int>(windowPos.x), static_cast<int>(windowPos.y));

	// Done creating the window
	ImGui::End();
	return;
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


