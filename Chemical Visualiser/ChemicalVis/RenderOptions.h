#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include <string>
#include "glad/glad.h"
#include "GlobalClass.h"

class RenderOptionsWindow
{
public:
	void Display();

	float FOV = 60.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	bool wireFrame = false;
	bool grid = false;
	float bgColour[3] = { 0.15f, 0.15f, 0.15f };
	float bgColour2[3] = { 0.8f, 0.8f, 0.8f};

	float viewPortDivider = 0.5f;

	float cameraSpeed = 5.0f; 
	float orthoScale = 0.1f;

private:
	void ListSettings();
};