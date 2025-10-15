#include "Model.h"

#include "raylib.h"

namespace Core::Model
{
	namespace Sphere
	{
		void Draw(float x, float y, float z, float radius, Color color) {
			::DrawSphere({ x, y, z }, radius, color);
		}
	}
}