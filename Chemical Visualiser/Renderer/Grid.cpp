#include "Grid.h"


Grid::Grid(int size, float scale)
{
	Grid::size = size;
	Grid::scale = scale;

	CalculateGrid();
	
	//axis lines
	glm::vec3 verts[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};
	GLuint indices[] =
	{
		0, 1,
		0, 2,
		0, 3
	};

	std::vector<glm::vec3> points(verts, verts + sizeof(verts) / sizeof(glm::vec3));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));


	axisLine = DebugLine(points, inds);
}

//this is slightly less efficient however not by that much that it is particularly worth redoing atm
void Grid::CalculateGrid()
{
	std::vector<glm::vec3> verts;
	std::vector<GLuint> inds;
	int totalVerts = size * 4;

	float x = 0.0f;
	float y = 0.0f;

	for (int i = 0; i < size + 1; i++)
	{
		int vertCount = (int)verts.size();
		verts.push_back(glm::vec3(x, 0, y));
		verts.push_back(glm::vec3(x, 0, y + size * scale));
		inds.push_back(vertCount);
		inds.push_back(vertCount+1);

		x += scale;
	}

	x = 0.0f;
	for (int i = 0; i < size + 1; i++)
	{
		int vertCount = (int)verts.size();
		verts.push_back(glm::vec3(x, 0, y));
		verts.push_back(glm::vec3(x + size * scale, 0, y));
		inds.push_back(vertCount);
		inds.push_back(vertCount + 1);

		y += scale;
	}

	gridLine = DebugLine(verts, inds, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f);
}

void Grid::Draw(Shader& gridShader, Shader& axisShader, Camera& camera)
{
	float mult = (float)(size * scale) / 2 ;
	gridLine.pos = glm::vec3(std::round(camera.position.x) - mult, 0, (std::round(camera.position.z) - mult));

	gridLine.Draw(gridShader, camera);

	axisLine.Draw(axisShader, camera);
	
}