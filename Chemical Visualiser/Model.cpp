#include "Model.h"

Model::Model(const char* path, std::vector<Texture> textures, glm::vec3 pos)
{
	// save data to class
	Model::dir = path;
	Model::textures = textures;
	Model::pos = pos;

	//load the model
	LoadModel();
}

void Model::LoadModel()
{
	// clear any prevoiously stored data
	// i won't do this atm as the function will only be used on a fresh mesh
	// 
	// import the mesh using assimp
	Assimp::Importer Importer;

	//the entire models data, all meshes
	const aiScene* scene = Importer.ReadFile(dir,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	//if there was something wrong return an error
	if (!scene)
	{
		std::cout << "ERROR: " << Importer.GetErrorString() << std::endl;
		return;
	}

	//this is where it would get textures if possible
	


	// loop through each mesh
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		// Proccess each mesh
		aiMesh* mesh = scene->mMeshes[i];
		ProccessMesh(mesh);
	}

}

void Model::ProccessMesh(aiMesh* mesh)
{
	//get all the vertices in an array
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	// Loop through each vertex
	for (int i = 0; i < mesh->mNumVertices;i++)
	{
		// get the position, normal and uv coordinates
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D norm = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);

		//put it all into a vertex
		Vertex vert = Vertex
		{
			glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(norm.x, norm.y,norm.z),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(uv.x, uv.y)
		};
		//add it to the vector of vertices
		vertices.push_back(vert);
	}

	//loop through each face
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];
		if (face.mNumIndices == 3)
		{
			//add the indices to the indices vector
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}

	// save data to mesh
	// and add it to the meshes vector
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::Draw(Shader& shader, Camera& camera, glm::vec3 translation)
{
	//define a matrix
	glm::mat4 matrix = glm::mat4(1.0f);
	//apply the translation to the position
	pos += translation;
	//set the matrix to the new position
	matrix = glm::translate(matrix, pos);

	//send the matrix to the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	for (int i = 0; i < meshes.size(); i++)
	{
		// this goes through each mesh and draws it to the screen
		// it is not very efficient as it costs 1 draw call per mesh
		// which very quickly adds up with lots of models
		meshes[i].Draw(shader, camera, matrix);
	}
}

