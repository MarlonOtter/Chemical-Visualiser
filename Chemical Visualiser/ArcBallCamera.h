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
	float radius = 1.0f;
	float speed = 3.0f;
	glm::vec3 cameraCentre;

	ArcBallCamera(int width, int height, glm::vec3 position);

	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);


	// set the position of camera based on user input
	void MouseInputs(ImGuiIO& io);
	void KeyInputs(ImGuiIO&io);

	glm::vec3 getPos();

	//Directional movements
	void Up(float value);
	void Down(float value);
	void Left(float value);
	void Right(float value);

	void Forward(float value);
	void Backward(float value);

	void MoveUp(float value);
	void MoveDown(float value);
	void MoveLeft(float value);
	void MoveRight(float value);

private:
	bool flag = false;
	glm::vec2 keyInputs;

	glm::mat4 viewMat;
	glm::vec2 mouseStartPos;

	glm::vec3 startPos;
	glm::vec3 currentPos = startPos;
	glm::vec3 startPosUnitVector;
	glm::vec3 currentPosUnitVector;

	Quaternion currentQuaternion;
	Quaternion lastQuaternion = { 0.0f, glm::vec3(1, 0, 0) };

	float cosValue, cosValue_2;
	float theta;
	float angle = 180.0f;

	glm::vec3 rotationalAxis = glm::vec3(1, 0, 0);
	glm::vec3 rotationalAxis_2;

	

	void rotation();

	glm::vec3 getUnitVector(glm::vec3 vectr);

	glm::mat4 calculateViewMatrix();
	float dotProduct();

	static float CalcZAxis(float x, float y, float radius);
	static glm::vec3 convertToNDC(glm::vec2 mousePos, int width, int height, float radius);


	
};