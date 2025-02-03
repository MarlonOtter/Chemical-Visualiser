#pragma once

#define PI 3.1416

#include "imgui/imgui.h"
#include "Renderer/Camera.h"

struct Quaternion {
	float cosine; //cosine of half the rotation angle
	glm::vec3 axis; //unit vector scaled by the sine of half the angle
};

class ArcBallCamera : public Camera
{
public:
	//speed that the camera moves at
	float speed = 3.0f;

	// the position that the camera moves about
	glm::vec3 cameraCentre;

	// Define the Camera
	ArcBallCamera(int width, int height, glm::vec3 position);

	// Update the camera matrix
	// Overides the original defined in camera.h
	// To use the new View Matrix
	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);

	// set the position of camera based on user input
	void MouseInputs(ImGuiIO& io);
	void KeyInputs(ImGuiIO&io);
	void ApplyInputs();

	// Get the position of the camera from its view matrix
	glm::vec3 getPos();


	// Directions
	// orbital movements
	void Up(float value);
	void Down(float value);
	void Left(float value);
	void Right(float value);

	// Zoom In/Out
	void Forward(float value);
	void Backward(float value);

	// Move Origin/Centre point
	void MoveUp(float value);
	void MoveDown(float value);
	void MoveLeft(float value);
	void MoveRight(float value);

private:

	//not sure what this does to be honest
	float radius = 1.0f;

	// Input vectors that get changed in their respective functions
	glm::vec2 mouseVec;
	glm::vec2 keyVec;

	// The view matrix so that the function getPos works
	glm::mat4 viewMat;
	
	// data needed for the arcball camera movement
	Quaternion lastQuaternion = { 0.0f, glm::vec3(1, 0, 0) };

	
	// Used to calculate the view matrix
	float angle = 180.0f;
	glm::vec3 rotationalAxis = glm::vec3(1, 0, 0);

	// Does all the rotation stuff
	void calculateRotation(glm::vec2 startVec, glm::vec2 crntVec);

	// Calculates the view matrix from a rotation
	// overide the original function in Camera.h
	glm::mat4 calculateViewMatrix();

	// Static functions
	
	// Calculate the Z from the x and y
	// From formula provieded here: https://oguz81.github.io/ArcballCamera/
	static float CalcZAxis(float x, float y, float radius);

	// Convert a mouse input vector to Normalised Device Coordinates 
	static glm::vec3 convertToNDC(glm::vec2 mousePos, int width, int height, float radius);

	// Calculates the unit (normalised) vector of a input vector
	static glm::vec3 getUnitVector(glm::vec3 vectr);

	
};