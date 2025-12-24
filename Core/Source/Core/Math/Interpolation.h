#pragma once
#include <algorithm>

namespace Core::Math
{
	inline float Lerp(float a, float b, float t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return a + t * (b - a);
	}

	inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
	{
		return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) };
	}

	inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
	{
		return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t) };
	}
}