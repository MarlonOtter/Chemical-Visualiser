#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "Model.h"

class Object
{
	public:
		std::string name;

		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	
		Object(Model* model, Shader* shader, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		Object(Model* model, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		Object(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		
		void SetName(std::string name);

		void SetModel(Model* model);
		void DelModel();

		void SetShader(Shader* shader);
		void DelShader();

		void Translate(glm::vec3 translation);
		void Rotate(glm::quat rotation);
		void Scale(glm::vec3 scale);

		void Draw(Shader& shader, Camera& camera);
		void Draw(Camera& camera);

		//void Delete();

		Model* GetModel();

	private:
		Model* model;
		Shader* shader;

		bool _modelExists();
		void ObjArrSetup();

};
#endif // !OBJECT_CLASS_H
