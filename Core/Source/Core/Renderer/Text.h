#pragma once

#include <string>
#include "Color.h"

namespace Core::Text
{
	void Draw(std::string text, int posX, int posY, int fontSize, Color color);
}