#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

// this is done through a online tutorial aswell as other online resources aswell as chatGPT for understanding certain functions
class Model
{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

		Model(const char* path, std::vector<Texture> textures, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		void Draw(Shader& shader, Camera& camera, glm::vec3 translation = glm::vec3(0.0f, 0.00f, 0.0f));

	private:

		const char* dir;
		std::vector<Texture> textures;
		std::vector<Mesh> meshes;
		
		void LoadModel();
		
		void ProccessMesh(aiMesh* mesh);
		std::vector<Texture> LoadMaterialTextures();
};

#endif // !MODEL_CLASS_H
