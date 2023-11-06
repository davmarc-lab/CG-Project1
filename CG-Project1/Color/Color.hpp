#pragma once
#include "../Lib.hpp"
#include <vector>

class Color
{

private:
	vec4 color;

public:
	Color() { this->color = vec4(0, 0, 0, 1); }

	Color(float red, float green, float blue) 
	{
		this->color = vec4(red < 0 || red > 1 ? 1.0 : red,
			green < 0 || green > 1 ? 1.0 : green,
			blue < 0 || blue > 1 ? 1.0 : blue, 1.0);
	}

	Color(float red, float green, float blue, float a)
	{
		this->color = vec4(red < 0 || red > 1 ? 1.0 : red,
			green < 0 || green > 1 ? 1.0 : green,
			blue < 0 || blue > 1 ? 1.0 : blue,
			a < 0 || a > 1 ? 1.0 : a);
	}

	Color(vec4 color) { this->color = vec4(color); }

	vec4 getColorVector() { return vec4(this->color); }

	void setColorVector(vec4 color) { this->color = vec4(color); }
};

// this namespace gives some standard colors
namespace color {
	const vec4 WHITE = vec4(1, 1, 1, 1);
	const vec4 BLACK = vec4(0, 0, 0, 1);
	const vec4 RED = vec4(1, 0, 0, 1);
	const vec4 GREEN = vec4(0, 1, 0, 1);
	const vec4 BLUE = vec4(0, 0, 1, 1);
}

