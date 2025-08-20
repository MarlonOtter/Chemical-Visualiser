
#include "ArcBallCamera.h"
#include <iostream>

ArcBallCamera::ArcBallCamera(int width, int height, glm::vec3 position)
	: camera(width, height, position), keyVec(0.0f, 0.0f), mouseVec(0.0f, 0.0f), viewMat(1.0f)
{
	//cameraCentre = glm::vec3(0.0f, 0.0f, 0.0f);
}

// calculate the cameraMatrix from projection matrix and view matrix
void ArcBallCamera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	camera.cameraMatrix = camera.calculateProjectionMatrix(FOVdeg, nearPlane, farPlane, 0) * calculateViewMatrix();
}


void ArcBallCamera::MouseInputs(ImGuiIO& io)
{
	camera.position.z *= 1 - io.MouseWheel * 0.1f;
	if (camera.position.z < -250.0f) camera.position.z = -250;
	radius = glm::abs(camera.position.z);
	
	// move camera position from mouse inputs
	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		//get mouse offset
		ImVec2 mouseOffset = io.MouseDelta;
		float multiplier = 0.05f;
		float moveSpeed = io.DeltaTime * glm::abs(camera.position.z) * multiplier;
		//move the centre point based on mouse offset
		MoveRight(mouseOffset.x * moveSpeed);
		MoveUp(-mouseOffset.y * moveSpeed);
	}

	// while the mouse button is held down
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		// add mouse offset to the movement vector
		mouseVec.x += io.MouseDelta.x;
		mouseVec.y += io.MouseDelta.y;
	}

	
}

void ArcBallCamera::KeyInputs(ImGuiIO& io)
{
	
	if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		// orbit controls
		// when the user is not holding Control
		float lookAmount = io.DeltaTime * 300.0f;
		if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) Up(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) Down(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) Left(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) Right(lookAmount);
	}
	else
	{
		// Zooming and panning

		//zooming 
		float zoomAmount = io.DeltaTime * 5.0f;
		if (ImGui::IsKeyDown(ImGuiKey_Minus)) Backward(zoomAmount);
		if (ImGui::IsKeyDown(ImGuiKey_Equal)) Forward(zoomAmount);

		//moving the centre point
		float multiplier = 0.5f;
		float moveAmount = io.DeltaTime * glm::abs(camera.position.z) * multiplier;
		if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) MoveUp(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) MoveDown(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) MoveLeft(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) MoveRight(moveAmount);
	}
}

void ArcBallCamera::ApplyInputs()
{
	// a constant (based on the viewport size)
	glm::vec2 cameraMiddle(camera.width / 2, camera.height / 2);

	// the vector that the mouse and key inputs create
	glm::vec2 moveVec = cameraMiddle + keyVec + mouseVec;

	// reset inputs otherwise it keeps on spinning
	keyVec = glm::vec2(0);
	mouseVec = glm::vec2(0);

	// If the user hasn't input anythingI don't need to calculate 
	// a new rotation as it hasn't changed since last frame
	if (moveVec == cameraMiddle) return;

	calculateRotation(cameraMiddle, moveVec);
	
}

//Directional movements
void ArcBallCamera::Up(float value)
{
	keyVec.y += value;
}

void ArcBallCamera::Down(float value)
{
	keyVec.y += -value;
}

void ArcBallCamera::Left(float value)
{
	keyVec.x += value;
}

void ArcBallCamera::Right(float value)
{
	keyVec.x += -value;
}

void ArcBallCamera::Forward(float value)
{
	camera.position.z += value;
}

void ArcBallCamera::Backward(float value)
{
	camera.position.z -= value;
}


// These need to be turned into relative movements to the camera rotation/position
void ArcBallCamera::MoveUp(float value)
{
	cameraCentre += -camera.upVec * value;
}

void ArcBallCamera::MoveDown(float value)
{
	cameraCentre += camera.upVec * value;
}

void ArcBallCamera::MoveLeft(float value)
{
	cameraCentre += camera.rightVec * value;
}

void ArcBallCamera::MoveRight(float value)
{;
	cameraCentre += -camera.rightVec * value;
}

glm::vec3 ArcBallCamera::getPos()
{
	//calculate the position of the camera from its view matrix
	return glm::vec3(glm::inverse(viewMat) * glm::vec4(glm::vec3(0), 1.0f));
}

void ArcBallCamera::calculateRotation(glm::vec2 startVec, glm::vec2 crntVec) 
{
	// Convert the start and current vectors to Normalised Device Coordinates
	glm::vec3 startPos = convertToNDC(startVec, camera.width, camera.height, radius);
	glm::vec3 currentPos = convertToNDC(crntVec, camera.width, camera.height, radius);

	// Calculate the unit/normalised vectors of the NDCs
	glm::vec3 startPosUnitVector = glm::normalize(startPos);
	glm::vec3 currentPosUnitVector = glm::normalize(currentPos);

	// Calculate the quaternions axis
	Quaternion currentQuaternion;
	currentQuaternion.axis = getUnitVector(glm::cross(startPos, currentPos));
	
	// Calculate cosValue, ensuring that it is not above one
	float cosValue = glm::min(glm::dot(startPosUnitVector, currentPosUnitVector), 1.0f);

	// Theta is the angle now
	// Multiply angle by speed to increase sensitvity of inputs
	float theta = static_cast<float>(acos(cosValue) * 180 / PI) * speed; 

	//currentQuaternion.cosine is cos of half the angle
	currentQuaternion.cosine = static_cast<float>(cos((theta / 2) * PI / 180)); 

	currentQuaternion.axis *= sin((theta / 2) * PI / 180);

	float cosValue_2 = (currentQuaternion.cosine * lastQuaternion.cosine) - glm::dot(currentQuaternion.axis, lastQuaternion.axis);

	glm::vec3 temporaryVector = glm::cross(currentQuaternion.axis, lastQuaternion.axis);


	glm::vec3 rotationalAxis_2;
	rotationalAxis_2 = (lastQuaternion.axis * currentQuaternion.cosine) + (lastQuaternion.cosine * currentQuaternion.axis) + temporaryVector;

	// Calculate the angle that will be rotated by
	angle = static_cast<float>((acos(cosValue_2) * 180 / PI) * 2);

	// Calculate the axis that will be rotated around
	rotationalAxis = rotationalAxis_2 / static_cast<float>(sin((angle / 2) * PI / 180));


	// Reset the values so that any axis can be rotated around the next frame
	lastQuaternion.cosine = cosValue_2;
	lastQuaternion.axis = rotationalAxis_2;
}


glm::mat4 ArcBallCamera::calculateViewMatrix()
{
	// Define the default matrix
	glm::mat4 view = glm::mat4(1.0f);

	//Move it to the centre position
	view = glm::translate(view, camera.position);

	// Rotate it around that point from what was calculated from inputs
	view = glm::rotate(view, glm::radians(angle), rotationalAxis);

	view = glm::translate(view, cameraCentre);

	// Store the matrix in the class
	viewMat = view;

	// Recalcualte the up and right vectors every frame
	// As they are used to move the center point that the camera orbits around
	glm::mat4 inverseView = glm::inverse(view);
	camera.upVec = glm::vec3(inverseView[1]);
	camera.rightVec = glm::vec3(inverseView[0]);

	// Return the matrix to the camera matrix function
	return view;
}


//Static Functions

//formula from https://oguz81.github.io/ArcballCamera/
float ArcBallCamera::CalcZAxis(float x, float y, float radius)
{
	float z = 0;
	if (sqrt((x * x) + (y * y)) <= radius) z = (float)sqrt((radius * radius) - (x * x) - (y * y));
	return z;
}

glm::vec3 ArcBallCamera::convertToNDC(glm::vec2 mousePos, int width, int height, float radius)
{
	glm::vec3 NDCpos;
	NDCpos.x = ((mousePos.x - (width) / 2) / (width / 2)) * radius;
	NDCpos.y = (((height / 2) - mousePos.y) / (height / 2)) * radius;
	NDCpos.z = CalcZAxis(NDCpos.x, NDCpos.y, radius);
	return NDCpos;
}

glm::vec3 ArcBallCamera::getUnitVector(glm::vec3 vectr) {
	float magnitude1;
	glm::vec3 unitVector;
	magnitude1 = (vectr.x * vectr.x) + (vectr.y * vectr.y) + (vectr.z * vectr.z);
	magnitude1 = sqrt(magnitude1);
	if (magnitude1 == 0) {
		unitVector.x = 0;
		unitVector.y = 0;
		unitVector.z = 0;
	}
	else {
		unitVector.x = vectr.x / magnitude1; //if magnitude 0, then this func give 'nan' error.
		unitVector.y = vectr.y / magnitude1;
		unitVector.z = vectr.z / magnitude1;
	}
	return unitVector;
}
