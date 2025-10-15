#include "Shape.h"

#include "raylib.h"

namespace Core::Shape
{
	namespace Circle
	{
		void Draw(int centerX, int centerY, int radius, Color color)
		{
			::DrawCircle(centerX, centerY, radius, color);
		}
	}
}