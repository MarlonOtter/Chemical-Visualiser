#include "Model.h"

Model::Model()
{

}

Model::Model(const char* path, std::vector<Texture> textures)
{
	// save data to class
	Model::dir = path;
	Model::textures = textures;

	//load the model
	LoadModel();
}

void Model::LoadModel()
{
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
	for (int i = 0; i < static_cast<int>(scene->mNumMeshes); i++)
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
	for (int i = 0; i < static_cast<int>(mesh->mNumVertices);i++)
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
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec2(uv.x, uv.y)
		};
		//add it to the vector of vertices
		vertices.push_back(vert);
	}

	//loop through each face
	for (int i = 0; i < static_cast<int>(mesh->mNumFaces); i++)
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

void Model::Draw(Shader& shader, Camera& camera, glm::vec3 pos, glm::quat rot, glm::vec3 scale)
{
	//define a matrix
	glm::mat4 matrix = glm::mat4(1.0f);
	//set the matrix to the desired position
	matrix = glm::translate(matrix, pos);

	//send the matrix to the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	for (int i = 0; i < meshes.size(); i++)
	{
		// this goes through each mesh and draws it to the screen
		// it is not very efficient as it costs 1 draw call per mesh
		// which very quickly adds up with lots of models
		glm::vec3 meshOffset = glm::vec3(0.0f, 0.0f, 0.0f);
		meshes[i].Draw(shader, camera, matrix, meshOffset, rot, scale);
	}

}	

void Model::DrawInstanced
(
	Shader& shader,
	Camera& camera,
	std::vector<glm::vec3> pos,
	std::vector<glm::quat> rot,
	std::vector<glm::vec3> scale
)
{
	//convert the vectors to matrices
	std::vector<glm::mat4> matrices = convertToMatrices(pos, rot, scale);

	//draw the instanced meshes
	DrawInstanced(shader, camera, matrices);
}

void Model::DrawInstanced
(
	Shader& shader,
	Camera& camera,
	std::vector<glm::mat4> matrices
)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].DrawInstanced(shader, camera, static_cast<int>(matrices.size()), matrices);
	}
}

// loop through each mesh and delete it
void Model::Delete()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Delete();
	}
	textures.clear();
	meshes.clear();
}



std::vector<glm::mat4> Model::convertToMatrices(std::vector<glm::vec3> pos, std::vector<glm::quat> rot, std::vector<glm::vec3> scale)
{
	std::vector<glm::mat4> matrices;
	//get the maximum number of items
	//incase that there are more of one than the other
	int maxSize = static_cast<int>(std::max(scale.size(), std::max(pos.size(), rot.size())));
	for (int i = 0; i < maxSize; i++)
	{
		//calculate matrix
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rotat = glm::mat4(1.0f);
		glm::mat4 scaling = glm::mat4(1.0f);

		//apply transformations to the matrix if able to
		if (i < pos.size())   trans = glm::translate(trans, pos[i]);
		if (i < rot.size())   rotat = glm::mat4_cast(rot[i]);
		if (i < scale.size()) scaling = glm::scale(scaling, scale[i]);

		//add matrix to list
		matrices.push_back(trans * rotat * scaling);
	}
	return matrices;
}