#include "Object.h"

// All the different variates of constructors
Object::Object(Model* model, Shader* shader, glm::vec3 pos)
{
	SetModel(model);
	SetShader(shader);
	Object::pos = pos;
	
	//Object::SetName("Object(" + std::to_string(model->objArr.size()) + ")");
}

Object::Object(Model* model, glm::vec3 pos)
{
	SetModel(model);
	Object::pos = pos;
	
	//Object::SetName("Object(" + std::to_string(model->objArr.size()) + ")");
}

Object::Object(glm::vec3 pos)
{
	Object::pos = pos;
	Object::name = "Object1";
}

void Object::SetName(std::string name)
{
	Object::name = name;
}

// set and delete model
void Object::SetModel(Model* model)
{
	Object::model = model;
}

void Object::DelModel()
{
	model = nullptr;
}

void Object::SetShader(Shader* shader)
{
	Object::shader = shader;
}

void Object::DelShader()
{
	shader = nullptr;
}

void Object::Translate(glm::vec3 translation)
{
	//move the object by a set value
	pos += translation;
}

void Object::Rotate(glm::quat rotation)
{
	// adding is incorrect so im just setting the value because that seems to work 
	Object::rotation *= rotation;
}

void Object::Scale(glm::vec3 scale)
{
	Object::scale *= scale; //does this want to multiply or add?
}

void Object::Draw(Shader& shader, Camera& camera)
{
	if (_modelExists())
	{
		//move model to object position and
		//draw it
		model->Draw(shader, camera, pos, rotation, scale);
		//the next object with the same model will then move it and draw it
	}
}

void Object::Draw(Camera& camera)
{
	if (shader == nullptr) return;
	Object::Draw(*shader, camera);
}

Model* Object::GetModel()
{
	//Checks if the model exists
	if (_modelExists())
	{
		//if it does return it as a reference
		return model;
	}
	return nullptr;
}

bool Object::_modelExists()
{
	//if the model doesn't have no value; return True
	return model != nullptr; 
}

void Object::ObjArrSetup()
{
	if (name == "")
	{
		//Object::SetName("Object(" + std::to_string(model->objArr.size()) + ")");
	}
	//model->objArr.Add(*this);
}