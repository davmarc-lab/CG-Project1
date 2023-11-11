#pragma once
#include "../Lib.hpp"
#include <vector>

/*
 * This class provides a structure to store RGBA colors using normalized values (0-1), also is possible
 *  to use the @namespace color to use some default colors.
 */
class Color
{

private:
    // Vector with 4 dimension to store the color elements.
	vec4 color;

public:
    // This empty constructor creates a standard color: black.
	Color() { this->color = vec4(0, 0, 0, 1); }
    
    // This constructor creates a color from red, green, blue values and define a default alpha.
	Color(float red, float green, float blue) 
	{
		this->color = vec4(red < 0 || red > 1 ? 1.0 : red,
			green < 0 || green > 1 ? 1.0 : green,
			blue < 0 || blue > 1 ? 1.0 : blue, 1.0);
	}

    // This constructor creates a color from rgba values.
	Color(float red, float green, float blue, float a)
	{
		this->color = vec4(red < 0 || red > 1 ? 1.0 : red,
			green < 0 || green > 1 ? 1.0 : green,
			blue < 0 || blue > 1 ? 1.0 : blue,
			a < 0 || a > 1 ? 1.0 : a);
	}

    // This constructor creates a color from a vec4 indicating (red, green, blue, alpha) values.
	Color(vec4 color) { this->color = vec4(color); }

    // Retrieves the color vector with the rgba format.
	vec4 getColorVector() { return vec4(this->color); }
    
    // Set the color vector from a vec4.
	void setColorVector(vec4 color) { this->color = vec4(color); }
};

/*
 * @namespace color
 * This namespace provides some basic colors.
 */
namespace color {
	const vec4 WHITE = vec4(1, 1, 1, 1);
	const vec4 BLACK = vec4(0, 0, 0, 1);
	const vec4 RED = vec4(1, 0, 0, 1);
	const vec4 GREEN = vec4(0, 1, 0, 1);
	const vec4 BLUE = vec4(0, 0, 1, 1);
}

