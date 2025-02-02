#pragma once

#include "imgui/imgui.h"
#include"Renderer/Camera.h"


class Camera2D : public Camera
{
public: 
	float scrollScale = 0.1f;

	Camera2D(int width, int height, glm::vec3 position);

	void UpdateMatrix(float nearPlane, float farPlane);

	

	void MouseInputs(ImGuiIO& io);
	void KeyInputs(ImGuiIO& io);

	void Forward(float value);
	void Backward(float value);
};