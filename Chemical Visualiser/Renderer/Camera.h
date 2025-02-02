#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 UpVec = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 cameraMatrix = glm::mat4(1.0f);


	float orthoScale = 0.05f;
	
	glm::vec2 viewportPos = glm::vec2(0);
	int width = 0;
	int height = 0;

	//for the input function
	double prevCursorPosX = 0.0;
	double prevCursorPosY = 0.0;
	bool firstClick = false;
	float sensitivity = 0.1f;

	Camera(int width, int height, glm::vec3 position);
	Camera();

	void UpdateSize(GLFWwindow* window);
	void UpdateSize(int width, int height);
	void UpdateSize(glm::vec2 pos, glm::vec2 size);
	void UpdateSize(glm::vec4 viewport);

	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, int view = 0);
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window, float deltaTime, float scrollOffset, float speed);

	void Up(float value);
	void Down(float value);
	void Left(float value);
	void Right(float value);
	void Forward(float value);
	void Backward(float value);

	glm::mat4 calculateViewMatrix();

	glm::mat4 calculateProjectionMatrix(float FOV, float near = 0.1f, float far = 100.0f, int view = 0);
	glm::mat4 calculateProjectionMatrix(float near = 0.1f, float far = 100.0f, int view = 1);
};
#endif // !CAMERA_CLASS_H
