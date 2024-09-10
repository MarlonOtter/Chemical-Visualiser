#include "EBO.h"

EBO::EBO(std::vector<GLuint>& indices)
{
	//create the buffer
	glGenBuffers(1, &ID);
	//bring it into context
	EBO::Bind();
	//add the data to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::Bind()
{
	//bring into current context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	//remove from current context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void EBO::Delete()
{
	//delete the buffer
	glDeleteBuffers(1, &ID);
}