#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include <string>

class EditorWindow
{
public:
	char ObjectID[64];
	float position[3];
	float scale[3] = {1.0f, 1.0f, 1.0f};
	float rotation[3];

	void Display();
	void setAll(std::string ID, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

private:
	float sliderVel = 0.01f;

	void DisplayTransforms();
	void DisplayData();
};