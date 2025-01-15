#ifndef OBJECT_ARRAY_CLASS_H
#define OBJECT_ARRAY_CLASS_H

#include "Object.h"

class ObjectArray
{
	public:

		static int Add(Object& object);

		static int Remove(int index);
		static int Remove(std::string ID);

		static Object* Get(int index);
		static Object* Get(std::string ID);

		static void Draw(Camera& camera);

		static int size();

	private:
		static std::vector<Object*> objArr;

		static int _findIndex(std::string ID);
};
#endif // !OBJECT_ARRAY_CLASS_H
