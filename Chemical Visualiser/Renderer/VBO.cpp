#include "VBO.h"

VBO::VBO()
{

}

VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	Unbind();
}

VBO::VBO(std::vector<glm::mat4>& mat4s)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
	Unbind();
}

VBO::VBO(std::vector<glm::vec3>& vec3s)
{
	glGenBuffers(1, &ID);
	Bind();
	//in the example it shows &transforms[0] however that differs from above
	glBufferData(GL_ARRAY_BUFFER, vec3s.size() * sizeof(glm::vec3), vec3s.data(), GL_STATIC_DRAW);
	Unbind();
}

VBO::~VBO()
{
	//Delete();
}

void VBO::SetData(std::vector<glm::mat4>& mat4s)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
	Unbind();
}

void VBO::SetData(std::vector<glm::vec3>& vec3s)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, vec3s.size() * sizeof(glm::vec3), vec3s.data(), GL_STATIC_DRAW);
	Unbind();
}


void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}