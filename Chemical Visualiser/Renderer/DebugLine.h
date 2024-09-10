#ifndef DEBUG_LINE_CLASS_H
#define DEBUG_LINE_CLASS_H

//
// This draws a line that is a pixel width
//





#include "VAO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include "glm/glm.hpp"
#include "Camera.h"

class DebugLine
{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 colour;

		std::vector<glm::vec3> points;
		std::vector<GLuint> indices;

		GLuint VAO;
		
		DebugLine();

		DebugLine(
			std::vector<glm::vec3> points,
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		
		
		DebugLine(
			std::vector<glm::vec3> points,
			std::vector<GLuint> indices,
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		void Draw(Shader& shader, Camera& camera);

		void SetPoints(std::vector<glm::vec3> points);

	private:
		void _Setup();
		bool hasIndices();
};

#endif // !LINE_CLASS_H
