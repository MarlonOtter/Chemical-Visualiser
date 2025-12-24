#pragma once

#include "raylib.h"
#include <vector>
#include <algorithm>

namespace Core
{
	using Color = ::Color;

	inline float Uint8ToFloat(uint8_t a)
	{
		return static_cast<float>(a) / 255.0f;
	}
	inline uint8_t FloatToUint8(float a)
	{
		return static_cast<uint8_t>(roundf(std::clamp(a, 0.0f, 1.0f) * 255.0f));
	}
}