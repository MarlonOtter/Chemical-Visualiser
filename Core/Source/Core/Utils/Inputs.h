#pragma once

#include "raylib.h"
#include "raymath.h"

namespace Core
{
	bool anyInputs()
	{
		return (
			GetMouseDelta() == Vector2{ 0,0 } &&
			GetMouseWheelMove() == 0 &&
			GetKeyPressed() == 0 &&
			IsMouseButtonUp(MOUSE_BUTTON_LEFT) &&
			IsMouseButtonUp(MOUSE_BUTTON_RIGHT) &&
			IsMouseButtonUp(MOUSE_BUTTON_MIDDLE)
			);
	}
}