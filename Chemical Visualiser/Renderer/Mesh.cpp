#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	// apply all the data to the class
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	construct();
}

// Construct the mesh
void Mesh::construct()
{
	
	VAO.Bind();

	//create the VBO and EBO
	VBO VBO(vertices);
	EBO EBO(indices);

	//define locations of data in vertex data
	//position
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//normal
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//colour
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//UV coordinate
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));

	//unbind everything so its cannot be changed
	VAO.Unbind();
	VBO.Unbind();
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

	//attach the camera position to the shader
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

	//apply camera transformations
	camera.Matrix(shader, "camMatrix");

	//calculate model matrix
	glm::mat4 transMatrix = glm::mat4(1.0f);
	glm::mat4 rotatMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	transMatrix = glm::translate(transMatrix, translation);
	rotatMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	//pass into the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(transMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rotatMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw the triangles to the screen
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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