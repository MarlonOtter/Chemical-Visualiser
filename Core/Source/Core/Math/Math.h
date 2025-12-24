#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include "Interpolation.h"

namespace Core::Math
{
	
	inline Vector2 Mean(std::vector<Vector2> arr)
	{
		Vector2 sum = { 0.0f, 0.0f };
		for (auto& v : arr)
		{
			sum += v;
		}
		return sum / static_cast<float>(arr.size());
	}

	inline float Mean(std::vector<float> arr)
	{
		float sum = 0.0f;
		for (auto& num : arr)
		{
			sum += num;
		}
		return sum / static_cast<float>(arr.size());
	}
}