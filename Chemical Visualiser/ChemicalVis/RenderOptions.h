#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include <string>
#include "glad/glad.h"

class RenderOptionsWindow
{
public:
	void Display();

	float FOV = 60.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	bool wireFrame;
	float bgColour[3] = { 0.15f, 0.15f, 0.15f };

	float cameraSpeed = 5.0f; 

private:
	void ListSettings();
};