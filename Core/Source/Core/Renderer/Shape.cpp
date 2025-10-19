#include "Shape.h"

#include "raylib.h"
#include "raymath.h"

namespace Core::Shape
{
	namespace Circle
	{
		void Draw(int centerX, int centerY, float radius, Color color)
		{
			::DrawCircle(centerX, centerY, radius, color);
		}
	}

	namespace Rect
	{
		void Draw(int x, int y, int width, int height, Color color)
		{
			::DrawRectangle(x, y, width, height, color);
		}

		void DrawPro(Rectangle rect, Vector2 origin, float rotation, Color color)
		{
			::DrawRectanglePro(rect, origin, rotation, color);
		}
	}

	namespace Line
	{
		void Draw(int x1, int y1, int x2, int y2, Color color)
		{
			::DrawLine(x1, y1, x2, y2, color);
		}

		void DrawEx(Vector2 P1, Vector2 P2, float width, Color color)
		{
			::DrawLineEx(P1, P2, width, color);
		}
	}
}