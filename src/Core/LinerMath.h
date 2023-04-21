#pragma once

#define NOMINMAX
#include <algorithm>

struct ver3f
{
	ver3f()
		: x(0.f), y(0.f), z(0.f) {}
	ver3f(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};