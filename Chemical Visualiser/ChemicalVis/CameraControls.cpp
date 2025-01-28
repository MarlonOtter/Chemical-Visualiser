#include "CameraControls.h"

#include <iostream>

glm::vec2 CameraControls::prevMousePos(0);
glm::vec2 CameraControls::mousePos(0);

glm::vec3 CameraControls::cameraOrbitCentre(0);

void CameraControls::Update()
{
	ImVec2 mousePos = ImGui::GetMousePos();
	CameraControls::mousePos = glm::vec2(mousePos.x, mousePos.y);
}

void CameraControls::Inputs3D(Camera& camera, ImGuiIO io) 
{
	//zoom in and out
	float scrollScale = 0.1f;
	float distance = glm::distance(camera.position, cameraOrbitCentre);
	camera.position += (camera.orientation * scrollScale) * io.MouseWheel * distance;

	if (ImGui::IsKeyPressed(ImGuiKey_H, false)) camera.position = glm::vec3(0);

	//return if the user is not dragging their mouse
	if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left)) return;

	//always look at orbit centre
	glm::vec3 diff = camera.position - cameraOrbitCentre;
	camera.orientation = glm::normalize(-diff);


	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		//Dragging to move
		return;
	}
	
	//normal orbit controls
}

//The users scroll zooms in and out of the 2D chemical
void CameraControls::Inputs2D(Camera& camera, ImGuiIO io)
{
	//arbitary value
	float scrollScale = 0.1f;
	camera.orthoScale *= 1.0f - (io.MouseWheel * scrollScale);
	
	//The mouse is being dragged
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
	{
		//calculate the distance the mouse has moved between this and the last frame
		glm::vec2 mouseOffset = glm::vec2(mousePos.x, mousePos.y) - prevMousePos;
		
		//apply a translation based on the mouse movement and how zoomed in the camera is.
		camera.position += glm::vec3(-mouseOffset.x, mouseOffset.y, 0.0f) * camera.orthoScale;
	}

	//This is for touchpads where the user can scroll horizontally
	float scrollX = io.MouseWheelH;
	camera.position += glm::vec3(-scrollX * camera.orthoScale * 100.0f, 0, 0);

	//set the prevous pos to the current pos. for next frame
	prevMousePos = mousePos;

}


glm::vec3 CameraControls::rotateAround(glm::vec3 pos, float degrees, glm::vec3 center, glm::vec3 axis)
{
	float angle = glm::radians(degrees);
	//translate pos to origin relative to center
	glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -center);
	//Apply rotation
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
	//translate back to position around center
	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);
	//combine transformations
	glm::mat4 transformation = translateBack * rotation * translateToOrigin;
	//apply to object
	return glm::vec3(transformation * glm::vec4(pos, 1.0f));
}