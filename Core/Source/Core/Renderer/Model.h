#pragma once

#include "Color.h"

namespace Core::Model
{
	namespace Sphere
	{
		void Draw(float x, float y, float z, float radius, Color color);
	}

	namespace Cylinder
	{
		void Draw(float x, float y, float z, float r1, float r2, float height, int slices, Color color);
		void DrawEx(Vector3 P1, Vector3 P2, float r1, float r2, int sides, Color color);
	}
}