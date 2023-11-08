#pragma once

#include <vector>
#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "ComplexShape2D.hpp"

class Shape2D : public ComplexShape2D
{
private:
    Color midColor = Color();
    Color color = Color();

public:

    Shape2D(int ntriangle, vector<vec3> vertex, vector<vec4> colors, mat4 model)
    {
        this->ntriangle = ntriangle;
        this->vertex = vertex;
        this->colors = colors;
        this->model = model;
        this->nvertex = this->vertex.size();
    }

    // Empty constructor
    Shape2D(int ntriangle)
    {
        this->ntriangle = ntriangle;
    }

    virtual void createVertexArray() override;

    virtual void draw() override;

};
