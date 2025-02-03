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


	// Directional movement
	// Overides the functions declared in Camera.h
	// changed to as i found the controls felt back-to-front for this usecase
	void Left(float value);
	void Right(float value);

	void Forward(float value);
	void Backward(float value);
};