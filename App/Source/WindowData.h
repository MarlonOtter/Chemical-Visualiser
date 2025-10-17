#pragma once

struct WindowData {
	int width = 400;
	int height = 400;
	bool hovered = false;
	bool focused = false;
	bool resizing = false;
	bool updated = false;
};
