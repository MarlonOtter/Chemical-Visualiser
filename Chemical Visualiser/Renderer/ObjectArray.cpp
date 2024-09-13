#include "ObjectArray.h"

ObjectArray::ObjectArray()
{
	// don't know if this will have to do anything
}

int ObjectArray::Add(Object& object)
{
	objArr.push_back(&object);
	return 1;
}

int ObjectArray::Remove(int index)
{
	if (index >= 0 && index < objArr.size())
	{
		objArr.erase(objArr.begin() + index);
		return 1;
	}
	return -1;
}

int ObjectArray::Remove(std::string name)
{
	if (objArr.size() < 0) return -1;

	return ObjectArray::Remove(_findIndex(name));
}

Object* ObjectArray::Get(int index)
{
	if (index < 0 || index > objArr.size()) return nullptr;
	return objArr[index];
}

Object* ObjectArray::Get(std::string name)
{
	return Get(_findIndex(name));
}

void ObjectArray::Draw(Camera& camera)
{
	for (int i = 0; i < objArr.size(); i++)
	{
		objArr[i]->Draw(camera);
	}
}

int ObjectArray::size()
{
	return (int)objArr.size();
}

int ObjectArray::_findIndex(std::string name)
{
	//this is just a simple linear search for the name
	for (int i = 0; i < objArr.size(); i++)
	{
		if (objArr[i]->name == name)// if the name is what is being looked for is found
		{
			return i; // return i
		}
	}
	return -1;
}