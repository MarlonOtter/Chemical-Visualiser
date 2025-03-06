
#include "Camera2D.h"

Camera2D::Camera2D(int width, int height, glm::vec3 position)
{
	camera = Camera(width, height, position);
}


void Camera2D::UpdateMatrix(float nearPlane, float farPlane)
{
	camera.UpdateMatrix(0.0f, nearPlane, farPlane, 1);
}


//INPUT METHODS

void Camera2D::MouseInputs(ImGuiIO& io)
{
	// Zoom in/out based on how much the user scroll
	Forward(io.MouseWheel * scrollScale);

	//The mouse is being dragged
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
	{
		//apply a translation based on the mouse movement and how zoomed in the camera is.
		camera.position += glm::vec3(-io.MouseDelta.x, io.MouseDelta.y, 0.0f) * camera.orthoScale;
	}

	//This is for touchpads where the user can scroll horizontally
	// not sure about including this
	camera.position += glm::vec3(-io.MouseWheelH * camera.orthoScale * 100.0f, 0, 0);
}

void Camera2D::KeyInputs(ImGuiIO& io)
{
	// These will be updated to use the keys in the settings however for now i will hardcode 
	// it until that is implemented

	float moveAmount = io.DeltaTime * 300.0f * camera.orthoScale;
	if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) camera.Up(moveAmount);
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) camera.Down(moveAmount);
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) Left(moveAmount);
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) Right(moveAmount);

	//zoom
	if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) return;

	float zoomAmount = io.DeltaTime * 1.5f;
	if (ImGui::IsKeyDown(ImGuiKey_Minus)) Backward(zoomAmount);
	if (ImGui::IsKeyDown(ImGuiKey_Equal)) Forward(zoomAmount);
}

void Camera2D::Left(float value)
{
	camera.position.x += -value;
}

void Camera2D::Right(float value)
{
	camera.position.x += value;
}

void Camera2D::Forward(float value)
{
	camera.orthoScale *= 1.0f - value;
}

void Camera2D::Backward(float value)
{
	camera.orthoScale *= 1.0f + value;
}