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

	namespace Cylinder
	{
		void Draw(float x, float y, float z, float r1, float r2, float height, int slices, Color color)
		{
			::DrawCylinder({ x,y,z }, r1, r2, height, slices, color);
		}

		void DrawEx(Vector3 P1, Vector3 P2, float r1, float r2, int sides, Color color)
		{
			::DrawCylinderEx(P1, P2, r1, r2, sides, color);
		}
	}
}