#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include <vector>

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

float dx(int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float dy(int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float DX(int i, float *t);

float DY(int i, float *t);

void hermiteInterpolation(float *t, glm::vec4 color_top, glm::vec4 color_bot, Curve *curve);

/**
 * @brief Interpolates the given points into mesh vertices.
 *
 * @param color_top the mesh top color
 * @param color_bot the mesh bot color
 * @param curve the data structure containing all control points and interpolated vertices
 */
void buildHermite(glm::vec4 color_top, glm::vec4 color_bot, Shared<Curve> curve);

// Read the text file given and store all the data.
/**
 * @brief Retrives a shared pointer to data read from the given file containing
 * a list of points to use for hermite interpolation.
 *
 * @param path the file path
 */
Shared<Curve> readDataFromFile(const char *path);
