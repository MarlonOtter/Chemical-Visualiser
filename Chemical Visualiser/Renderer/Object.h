#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include "Model.h"

class Object
{
	public:
		Model* model;
		Shader* shader;
		std::string ID;

		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	
		Object(std::string ID, Model* model, Shader* shader, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		Object(std::string ID, Model* model, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		Object(std::string ID, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		
		void SetID(std::string ID);

		void SetModel(Model* model);
		void DelModel();

		void SetShader(Shader* shader);
		void DelShader();
		Shader& getShader();

		void Translate(glm::vec3 translation);
		void Rotate(glm::quat rotation);
		void Scale(glm::vec3 scale);

		void Draw(Shader& shader, Camera& camera);
		void Draw(Camera& camera);

		//void Delete();

		Model* GetModel();

	private:
		

		bool _modelExists();
		void ObjArrSetup();

};
#endif // !OBJECT_CLASS_H
