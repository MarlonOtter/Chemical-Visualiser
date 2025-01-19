#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

// this is done through a online tutorial aswell as other online resources aswell as chatGPT for understanding certain functions
class Model
{
	public:
		std::vector<Mesh> meshes;

		Model(const char* path, std::vector<Texture> textures = std::vector<Texture>());

		void Draw
		(
			Shader& shader,
			Camera& camera,
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f)
		);

		void DrawInstanced
		(
			Shader& shader,
			Camera& camera,
			std::vector<glm::vec3> pos = { glm::vec3(0.0f, 0.0f, 0.0f) },
			std::vector<glm::quat> rot = { glm::quat(1.0f, 0.0f, 0.0f, 0.0f) },
			std::vector<glm::vec3> scale = { glm::vec3(0.0f, 0.0f, 0.0f) }
		);

		void DrawInstanced
		(
			Shader& shader,
			Camera& camera,
			std::vector<glm::mat4> matrices
		);
		
		void Delete();
	private:

		const char* dir;
		std::vector<Texture> textures;
		
		
		void LoadModel();
		
		void ProccessMesh(aiMesh* mesh);
		std::vector<Texture> LoadMaterialTextures();

};

#endif // !MODEL_CLASS_H
