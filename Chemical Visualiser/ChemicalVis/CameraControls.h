#pragma once
#include "../Renderer/Camera.h"
#include "Settings.h"
#include "imgui.h"

class CameraControls 
{
public:
	static void Update();
	//Mouse and keyboard controls
	static void Inputs3D(Camera& camera, ImGuiIO io);
	static void Inputs2D(Camera& camera, ImGuiIO io);
private:
	static glm::vec2 prevMousePos;
	static glm::vec2 mousePos;

	static glm::vec3 cameraOrbitCentre;

	static glm::vec3 rotateAround(glm::vec3 pos, float degrees, glm::vec3 center, glm::vec3 axis);
};