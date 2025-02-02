#include "Camera.h"

#include <iostream>

Camera::Camera()
{

}

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;
}

void Camera::UpdateSize(GLFWwindow* window)
{
	// get the size of the window
	glfwGetFramebufferSize(window, &width, &height);
}

void Camera::UpdateSize(int width, int height)
{
	Camera::width = width;
	Camera::height = height;
}

void Camera::UpdateSize(glm::vec4 viewport)
{
	Camera::viewportPos = glm::vec2(viewport.x, viewport.y);
	width = viewport.z;
	height = viewport.w;
}

void Camera::UpdateSize(glm::vec2 pos, glm::vec2 size)
{
	viewportPos = pos;
	width = size.x;
	height = size.y;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int view)
{
	//multiply the matrices into one
	cameraMatrix = calculateProjectionMatrix(FOVdeg, nearPlane, farPlane, view) * calculateViewMatrix();
}


void Camera::Matrix(Shader& shader, const char* uniform)
{
	//pass the camera matrix into the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime, float scrollOffset, float speed)
{
	//handle keyboard inputs for the camera

	//WASD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Forward(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Left(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Backward(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Right(speed * deltaTime);
	}
	
	//Up and down
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Up(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Down(speed * deltaTime);
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
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, UpVec)));

		//make so that the camera can't just do barrel rolls (limits how far up ypu can look)
		if (!((glm::angle(newOrientation, UpVec) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -UpVec) <= glm::radians(5.0f))))
		{
			orientation = newOrientation;
		}

		//rotate the camera around the Y axis the desired amount
		orientation = glm::rotate(orientation, glm::radians(-rotY), UpVec);
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

void Camera::Up(float value)
{
	position += UpVec * value;
}

void Camera::Down(float value)
{
	position += -UpVec * value;
}

void Camera::Left(float value)
{
	position += -glm::normalize(glm::cross(orientation, UpVec)) * value;
}

void Camera::Right(float value)
{
	position += glm::normalize(glm::cross(orientation, UpVec)) * value;
}

void Camera::Forward(float value)
{
	position += orientation * value;
}

void Camera::Backward(float value)
{
	position += -orientation * value;
}


glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + orientation, UpVec);
}

glm::mat4 Camera::calculateProjectionMatrix(float FOV, float near, float far, int view)
{
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	if (width == 0 || height == 0) aspectRatio = 1.0f;

	//calculate the matrix for perspective
	if (view == 0) return glm::perspective(glm::radians(FOV), aspectRatio, near, far);

	//if the view is 1 : calculate the orthographic projection matrix
	if (view == 1) return glm::ortho(-0.5f * (float)width * orthoScale, (float)width * orthoScale * 0.5f, -0.5f * (float)height * orthoScale, (float)height * orthoScale * 0.5f, float(near), float(far));
}

glm::mat4 Camera::calculateProjectionMatrix(float near, float far, int view)
{
	return calculateProjectionMatrix(60.0f, near, far, view);
}

