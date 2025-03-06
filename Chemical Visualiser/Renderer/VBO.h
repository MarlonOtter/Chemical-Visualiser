#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 texUV;
};

class VBO
{
	public:
		GLuint ID;
		
		VBO();
		VBO(std::vector<Vertex>& vertices);
		VBO(std::vector<glm::mat4>& mat4s);
		VBO(std::vector<glm::vec3>& vec3s);

		// Deconstructor
		~VBO();

		void SetData(std::vector<glm::mat4>& mat4s);
		void SetData(std::vector<glm::vec3>& vec3s);

		void Bind();
		void Unbind();
		void Delete();
};
#endif // !VBO_CLASS_H
