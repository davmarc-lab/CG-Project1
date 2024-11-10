#pragma once

#include "../../Bolt-Core/include/Engine.hpp"
#include "../../Bolt-Core/include/Graphics.hpp"

#include <vector>

#define PHI0(t) (2.0 * t * t * t - 3.0 * t * t + 1)
#define PHI1(t) (t * t * t - 2.0 * t * t + t)
#define PSI0(t) (-2.0 * t * t * t + 3.0 * t * t)
#define PSI1(t) (t * t * t - t * t)

using namespace bolt;

struct Curve {
    u16 nv = 0;
    std::vector<vec3> vertex{};
    std::vector<vec4> colors{};
    std::vector<vec3> CP{};
    std::vector<vec4> colCP{};
};

float dx(int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float dy(int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float DX(int i, float *t);

float DY(int i, float *t);

void hermiteInterpolation(float *t, vec4 color_top, vec4 color_bot, Curve *curve);

// Creates the shapes from the already read text file.
void buildHermite(vec4 color_top, vec4 color_bot, Curve *curve);

// Read the text file given and store all the data.
Shared<Curve> readDataFromFile(const char *path);

void initHermiteMesh(const u32& id, Curve *mesh);
