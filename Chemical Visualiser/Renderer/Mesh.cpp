#include "Mesh.h"

Mesh::Mesh(
	std::vector<Vertex>& vertices,
	std::vector<GLuint>& indices,
	std::vector<Texture>& textures,
	bool instancing
)
{
	// apply all the data to the class
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
	Mesh::instancing = instancing;

	construct();
}

// Construct the mesh
void Mesh::construct()
{
	
	VAO.Bind();

	//create the VBO and EBO
	VBO VBO1(vertices);
	EBO EBO(indices);

	//define locations of data in vertex data
	//position
	VAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//normal
	VAO.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//colour
	VAO.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//UV coordinate
	VAO.LinkAttrib(VBO1, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));

	//unbind everything so its cannot be changed
	VAO.Unbind();
	VBO1.Unbind();
	EBO.Unbind();
}

// send the mesh data to the shader
void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	shader.Activate();
	VAO.Bind();

	// If there any textures add them to the shader
	if (textures.size() > 0) AddTextures(shader);

	//attach the camera position to the shader
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

	//apply camera transformations
	camera.Matrix(shader, "camMatrix");

	CalculateMatrices(shader, translation, rotation, scale);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw the triangles to the screen
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawInstanced
(
	Shader& shader,
	Camera& camera,
	int numInstances,
	std::vector<glm::mat4> matrices,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	//i could put all this in a seperate function becuase it is shared between the two functions
	shader.Activate();
	VAO.Bind();

	// If there any textures add them to the shader
	if (textures.size() > 0) AddTextures(shader);

	//attach the camera position to the shader
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

	//apply camera transformations
	camera.Matrix(shader, "camMatrix");
	CalculateMatrices(shader, translation, rotation, scale);


	//add the matrices for all the object instances
	VBO instanceVBO(matrices);

	instanceVBO.Bind();

	//link the matrices to the VAO
	VAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
	VAO.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	VAO.LinkAttrib(instanceVBO, 8, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	//make so that data is only refeshed every instances instead of every vertex
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);

	instanceVBO.Unbind();


	//draw the instances
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, matrices.size());
	instanceVBO.Delete();
}


void Mesh::Delete()
{
	VAO.Delete();
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].Delete();
	}
	textures.clear();
}

void Mesh::AddTextures(Shader& shader)
{
	shader.Activate();
	//variables that act as a counter for the number of each type of textures
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	//loop through each texture
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		//get the textures type
		std::string type = textures[i].type;
		//if its diffues increment the diffuse counter
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		//if its specular increment the specular counter
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		//apply the texture to the shader named after its type and the number of that type
		textures[i].texUnit(shader, (type + num).c_str(), i);
		//bind the texture
		textures[i].Bind();
	}
}

void Mesh::CalculateMatrices
(
	Shader& shader,
	glm::vec3& translation,
	glm::quat& rotation,
	glm::vec3& scale
)
{
	//calculate model matrix
	glm::mat4 transMatrix = glm::mat4(1.0f);
	glm::mat4 rotatMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	transMatrix = glm::translate(transMatrix, translation);
	rotatMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	shader.Activate();
	//pass into the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "meshMat4"), 1, GL_FALSE, glm::value_ptr(transMatrix));
}