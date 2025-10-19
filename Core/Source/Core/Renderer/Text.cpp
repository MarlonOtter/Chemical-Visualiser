#include "Text.h"

#include "raylib.h"

namespace Core::Text
{
	void Draw(std::string text, int posX, int posY, int fontSize, Color color)
	{
		::DrawText(text.c_str(), posX, posY, fontSize, color);
	}

	int Measure(std::string text, int fontSize)
	{
		return ::MeasureText(text.c_str(), fontSize);
	}
}