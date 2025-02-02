
#include "ArcBallCamera.h"
#include <iostream>

ArcBallCamera::ArcBallCamera(int width, int height, glm::vec3 position)
{
	ArcBallCamera::width = width;
	ArcBallCamera::height = height;
	ArcBallCamera::position = position;
}

//don't know if i need to redefine this but i will try removing it when finished
void ArcBallCamera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	cameraMatrix = calculateProjectionMatrix(FOVdeg, nearPlane, farPlane, 0) * calculateViewMatrix();
}


void ArcBallCamera::MouseInputs(ImGuiIO& io)
{
	position.z *= 1 - io.MouseWheel * 0.1f;
	radius = glm::abs(position.z);

	// camera translation

	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		//get mouse offset
		ImVec2 mouseOffset = io.MouseDelta;
		position.x += mouseOffset.x * io.DeltaTime * 1.0f;
		position.y += -mouseOffset.y * io.DeltaTime * 1.0f;
	}



	// arcball Camera movement
	
	//get the mouse position
	ImVec2 mousePos = ImGui::GetMousePos();

	// when the user first clicks the left mouse button
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		// convert to normalised device coordinates (NDC)
		startPos = convertToNDC(glm::vec2(mousePos.x, mousePos.y), width, height, radius);
		flag = true;
	}

	// while the mouse button is held down
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		// convert the current mouse position to NDC
		currentPos = convertToNDC(glm::vec2(mousePos.x, mousePos.y), width, height, radius);

		// Apply rotation
		rotation();
	}
	

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		keyInputs = glm::vec2(0);
		lastQuaternion.cosine = cosValue_2;
		lastQuaternion.axis = rotationalAxis_2;
		flag = false;
	}
}

void ArcBallCamera::KeyInputs(ImGuiIO& io)
{

	if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		float lookAmount = io.DeltaTime * 300.0f;
		if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) Up(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) Down(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) Left(lookAmount);
		if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) Right(lookAmount);
	}
	else
	{
		//zooming 
		float zoomAmount = io.DeltaTime * 5.0f;
		if (ImGui::IsKeyDown(ImGuiKey_Minus)) Backward(zoomAmount);
		if (ImGui::IsKeyDown(ImGuiKey_Equal)) Forward(zoomAmount);

		//moving the centre point
		float moveAmount = io.DeltaTime * -7.5f;
		if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) MoveUp(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) MoveDown(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) MoveLeft(moveAmount);
		if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) MoveRight(moveAmount);
	}
}

//Directional movements
void ArcBallCamera::Up(float value)
{
	keyInputs.y += value;
}

void ArcBallCamera::Down(float value)
{
	keyInputs.y += -value;
}

void ArcBallCamera::Left(float value)
{
	keyInputs.x -= value;
}

void ArcBallCamera::Right(float value)
{
	keyInputs.x += value;
}

void ArcBallCamera::Forward(float value)
{
	position.z += value;
}

void ArcBallCamera::Backward(float value)
{
	position.z -= value;
}


void ArcBallCamera::MoveUp(float value)
{
	position.y += value;
}

void ArcBallCamera::MoveDown(float value)
{
	position.y += -value;
}

void ArcBallCamera::MoveLeft(float value)
{
	position.x += -value;
}

void ArcBallCamera::MoveRight(float value)
{
	position.x += value;
}



glm::vec3 ArcBallCamera::getPos()
{
	//calculate the position of the camera from its view matrix
	return glm::vec3(glm::inverse(viewMat) * glm::vec4(glm::vec3(0), 1.0f));
}


void ArcBallCamera::rotation() {
	//now i can simplify this, testing that i works every now and then
	
	startPosUnitVector = glm::normalize(startPos);
	currentPosUnitVector = glm::normalize(currentPos);
	currentQuaternion.axis = getUnitVector(glm::cross(startPos, currentPos));
	//currentQuaternion.axis = getUnitVector(currentQuaternion.axis); //something wrong with just normalising this using GLM, causes it to flicker
	
	cosValue = glm::dot(startPosUnitVector, currentPosUnitVector);

	if (cosValue > 1) cosValue = 1; // when dot product gives '1' as result, it isn't equal to 1. It is very close to one: 1.00000000001 . 
	theta = (acos(cosValue) * 180 / PI) * speed; //theta is the angle now. multiply angle by speed to increase sens
	currentQuaternion.cosine = cos((theta / 2) * PI / 180); //currentQuaternion.cosine is cos of half the angle now.

	currentQuaternion.axis *= sin((theta / 2) * PI / 180);

	cosValue_2 = (currentQuaternion.cosine * lastQuaternion.cosine)
		- glm::dot(currentQuaternion.axis, lastQuaternion.axis);


	glm::vec3 temporaryVector;

	temporaryVector = glm::cross(currentQuaternion.axis, lastQuaternion.axis);


	rotationalAxis_2.x = (currentQuaternion.cosine * lastQuaternion.axis.x) +
		(lastQuaternion.cosine * currentQuaternion.axis.x) +
		temporaryVector.x;

	rotationalAxis_2.y = (currentQuaternion.cosine * lastQuaternion.axis.y) +
		(lastQuaternion.cosine * currentQuaternion.axis.y) +
		temporaryVector.y;

	rotationalAxis_2.z = (currentQuaternion.cosine * lastQuaternion.axis.z) +
		(lastQuaternion.cosine * currentQuaternion.axis.z) +
		temporaryVector.z;

	angle = (acos(cosValue_2) * 180 / PI) * 2;

	rotationalAxis = rotationalAxis_2 / static_cast<float>(sin((angle / 2) * PI / 180));
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




glm::mat4 ArcBallCamera::calculateViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);//Camera::calculateViewMatrix();
	view = glm::translate(view, position);// this, too.  
	view = glm::rotate(view, glm::radians(angle), rotationalAxis);
	//view = glm::translate(view, cameraCentre);
	viewMat = view;

	return view;
}

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
