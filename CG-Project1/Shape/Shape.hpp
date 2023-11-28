#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "ComplexShape2D.hpp"

class Shape2D : public ComplexShape2D
{

public:

    Shape2D(const Shape2D* shape)
    {
        this->ntriangle = shape->ntriangle;
        this->vertex = shape->vertex;
        this->colors = shape->colors;
        this->model = mat4(1.0f);
        this->createVertexArray();
        this->nvertex = this->vertex.size();
    }

    // Empty constructor
    Shape2D(int ntriangle)
    {
        this->ntriangle = ntriangle;
    }

    void setColor(Color color) { this->color = color; }

    void setMidColor(Color mid) { this->midColor = color; }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    virtual void clearShape() override;

};
