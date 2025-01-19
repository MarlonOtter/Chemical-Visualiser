#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

//files
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		bool instancing;

		VAO VAO;

		Mesh(
			std::vector<Vertex>& vertices,
			std::vector<GLuint>& indices,
			std::vector<Texture>& textures,
			bool instancing = false
		);

		void construct();
		void load(const char* filePath);
		void Draw
		(
			Shader& shader,
			Camera& camera,
			glm::mat4 matrices = glm::mat4(1.0f),
			//offset for the individual mesh
			glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		void DrawInstanced
		(
			Shader& shader,
			Camera& camera,
			int numInstances = 1,
			std::vector<glm::mat4> matrix = { glm::mat4(1.0f) },
			glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		void Delete();
	private:
		void AddTextures(Shader& shader);

		void CalculateMatrices
		(
			Shader& shader,
			glm::vec3& translation,
			glm::quat& rotation,
			glm::vec3& scale
		);
};

#endif // !MESH_CLASS_H
