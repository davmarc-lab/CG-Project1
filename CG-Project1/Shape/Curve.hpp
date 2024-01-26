#pragma once

#include "../Lib.hpp"
#include "ComplexShape2D.hpp"

#define PHI0(t) (2.0 * t * t * t - 3.0 * t * t + 1)
#define PHI1(t) (t * t * t - 2.0 * t * t + t)
#define PSI0(t) (-2.0 * t * t * t + 3.0 * t * t)
#define PSI1(t) (t * t * t - t * t)

// This class creates a shape doing hermite interpolation of control points written in a file.
class Curve : public ComplexShape2D
{
    public:
        vector<vec3> CP;
        vector<vec4> colCP;

        float dx (int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

        float dy (int i, float *t, float Tens, float Bias, float Cont, Curve *shape);

        float DX (int i, float *t);

        float DY (int i, float *t);

        void hermiteInterpolation(float *t, vec4 color_top, vec4 color_bot, Curve* fig);

    public:
        // Creates the shapes from the already read text file.
        void buildHermite(vec4 color_top, vec4 color_bot);

        // Read the text file given and store all the data.
        void readDataFromFile(const char* pathFile);

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void clearShape() override;

        virtual void runAction() override {}
};
