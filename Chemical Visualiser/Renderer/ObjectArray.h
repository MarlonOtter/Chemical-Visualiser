#ifndef OBJECT_ARRAY_CLASS_H
#define OBJECT_ARRAY_CLASS_H

#include "Object.h"

class ObjectArray
{
	public:
		ObjectArray();

		int Add(Object& object);

		int Remove(int index);
		int Remove(std::string ID);

		Object* Get(int index);
		Object* Get(std::string ID);

		void Draw(Camera& camera);

		int size();

	private:
		std::vector<Object*> objArr;

		int _findIndex(std::string ID);
};
#endif // !OBJECT_ARRAY_CLASS_H
