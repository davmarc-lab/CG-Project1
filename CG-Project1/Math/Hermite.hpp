#pragma once

#include "../Shape/Curve.hpp"

#define PHI0(t) (2.0 * t * t * t - 3.0 * t * t + 1)
#define PHI1(t) (t * t * t - 2.0 * t * t + t)
#define PSI0(t) (-2.0 * t * t * t + 3.0 * t * t)
#define PSI1(t) (t * t * t - t * t)

float dx (int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float dy (int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

float DX (int i, float *t);

float DY (int i, float *t);

Curve* hermiteInterpolation(float *t, Curve *shape, vec4 color_top, vec4 color_bot);

Curve* buildHermite(vec4 color_top, vec4 color_bot, Curve *shape);

void readDataFromFile(const char* pathFile);
