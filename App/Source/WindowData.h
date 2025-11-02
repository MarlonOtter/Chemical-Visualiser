#pragma once
#include "imgui.h"


struct WindowData {
	int width = 400;
	int height = 400;
	bool hovered = false;
	bool focused = false;
	bool resizing = false;
	bool closed = false;
	bool dockChange = false;
};
