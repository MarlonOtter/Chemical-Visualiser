#pragma once

#include "Renderer/Model.h"

class StartupScreen
{
public:
	static void Display(GLFWwindow* window);

private:
	static std::string imgAddr;

	static void Draw(GLFWwindow* window);
	static void UpdateScreen(GLFWwindow* window);
};