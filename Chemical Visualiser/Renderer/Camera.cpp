#include "Camera.h"

#include <iostream>

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;

	speed = baseSpeed;
}

void Camera::UpdateSize(GLFWwindow* window)
{
	// get the size of the window
	glfwGetFramebufferSize(window, &width, &height);
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	//define the matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	//move the camera away so the model can be seen better
	view = glm::lookAt(position, position + orientation, Up);

	//calculate the aspect ratio of the screen
	// * encountered issue due to not being turned to floats so it was doing integer division *
	
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	if (width == 0 || height == 0) aspectRatio = 1.0f;

	//make so there is perspective
	proj = glm::perspective(glm::radians(FOVdeg), aspectRatio, nearPlane, farPlane);
	

	//multiply the matrices into one
	cameraMatrix = proj * view;
}


void Camera::Matrix(Shader& shader, const char* uniform)
{
	//pass the camera matrix into the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime, float scrollOffset)
{
	//handle keyboard inputs for the camera

	//WASD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * deltaTime * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * deltaTime * -glm::normalize(glm::cross(orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * deltaTime * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * deltaTime * glm::normalize(glm::cross(orientation, Up));
	}
	
	//Up and down
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += speed * deltaTime * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		position += speed * deltaTime * -Up;
	}

	//mouse control
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//if the mouse button was only just pressed it sets the previous location as the currnet
		//so that the camera doesn't jump about
		if (firstClick)
		{
			//gets the mouse position
			glfwGetCursorPos(window, &prevCursorPosX, &prevCursorPosY);
			firstClick = false;
		}

		//mouse position variables
		double mouseX;
		double mouseY;
		//gets the mouse position
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//calculate the rotation values in the X and Y
		float rotX = sensitivity * (float)(mouseY - prevCursorPosY);
		float rotY = sensitivity * (float)(mouseX - prevCursorPosX);

		//calculate the new orientation
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, Up)));

		//make so that the camera can't just do barrel rolls (limits how far up ypu can look)
		if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
		{
			orientation = newOrientation;
		}

		//rotate the camera around the Y axis the desired amount
		orientation = glm::rotate(orientation, glm::radians(-rotY), Up);
		//sets the mouse to the center of the window
		glfwGetCursorPos(window, &prevCursorPosX, &prevCursorPosY);
	}
	//you let go of the left mouse button
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		//set value to true so that it can be reused
		firstClick = true;
	}
}

