#include "DebugLine.h"
#include <iostream>

DebugLine::DebugLine()
{

}

DebugLine::DebugLine(std::vector<glm::vec3> points, glm::vec3 pos, glm::vec3 colour)
{
	DebugLine::points = points;
	DebugLine::pos = pos;
	DebugLine::colour = colour;

	_Setup();
}

// couldn't work out how to use the EBO to do this
DebugLine::DebugLine(std::vector<glm::vec3> points, std::vector<GLuint> indices, glm::vec3 pos, glm::vec3 colour)
{
	DebugLine::points = points;
	DebugLine::indices = indices;
	DebugLine::pos = pos;
	DebugLine::colour = colour;

	_Setup();
}

DebugLine::~DebugLine()
{
	Delete();
}

void DebugLine::_Setup()
{
	std::cout << "WARNING: This Code Contains a memory leak and should not be ran until Fixed\n";
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO stuff
	VBO VBO1(points);
	vbo = &VBO1;

	//EBO
	if (hasIndices())
	{
		EBO EBO1(indices);
		ebo = &EBO1;
	}

	//define the layout of the data in the VBO
	vbo->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind the VAO, VBO
	glBindVertexArray(0);
	vbo->Unbind();
}

void DebugLine::Draw(Shader& shader, Camera& camera)
{

	// activate shader program
	shader.Activate();
	// Bind VAO
	glBindVertexArray(VAO);

	// pass in the camera and model matrix 
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);

	camera.Matrix(shader, "camMatrix");
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// pass in the colour
	glUniform3f(glGetUniformLocation(shader.ID, "colour"), colour.r, colour.g, colour.b);
	//pass in camera pos
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

	// Draw Lines
	
	if (hasIndices())
	{
		//draw elements makes it use the index buffer instead of just the vertices
		glDrawElements(GL_LINES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
	}
	else
	{
		// this only uses the vertices so extra effort is not needed to define the indices
		// however it will just draw them one after anouther
		glDrawArrays(GL_LINE_STRIP, 0, static_cast<int>(points.size()));
	}
	
}

void DebugLine::SetPoints(std::vector<glm::vec3> points)
{
	
}

void DebugLine::Delete()
{
	points.clear();
	indices.clear();
	glDeleteVertexArrays(1, &VAO);
	vbo->Delete();
	if (hasIndices())
	{
		ebo->Delete();
	}
}

bool DebugLine::hasIndices()
{
	return indices.size() > 0;
}

