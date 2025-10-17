#pragma once

#include "Color.h"

namespace Core::Shape
{
	namespace Circle
	{
		void Draw(int centerX, int centerY, float radius, Color color);
	}

	namespace Rect
	{
		void Draw();
	}

	namespace Line
	{
		void Draw(int x1, int y1, int x2, int y2, Color color);
		void DrawEx(Vector2 P1, Vector2 P2, float width, Color color);
	}
}