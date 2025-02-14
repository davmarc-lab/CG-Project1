#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include <vector>

#include "../include/Entity.hpp"

#define PHI0(t) (2.0 * t * t * t - 3.0 * t * t + 1)
#define PHI1(t) (t * t * t - 2.0 * t * t + t)
#define PSI0(t) (-2.0 * t * t * t + 3.0 * t * t)
#define PSI1(t) (t * t * t - t * t)

struct Curve {
	int nv = 0;
	std::vector<glm::vec3> vertex{};
	std::vector<glm::vec4> colors{};
	std::vector<glm::vec3> CP{};
	std::vector<glm::vec4> colCP{};
};

float dx(int i, float* t, float Tens, float Bias, float Cont, Curve* shape);

float dy(int i, float* t, float Tens, float Bias, float Cont, Curve* shape);

float DX(int i, float* t);

float DY(int i, float* t);

void hermiteInterpolation(float* t, glm::vec4 color_top, glm::vec4 color_bot, Curve* curve);

// Creates the shapes from the already read text file.
void buildHermite(glm::vec4 color_top, glm::vec4 color_bot, Shared<Curve> curve);

// Read the text file given and store all the data.
Shared<Curve> readDataFromFile(const char* path);
