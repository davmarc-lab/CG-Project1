#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "ComplexShape2D.hpp"

/*
 * This class instance a generic shape implementing ComplexShape2D interface.
 */
class Shape2D : public ComplexShape2D
{

public:

    // Constructs a copy of the given shape.
    Shape2D(const Shape2D* shape)
    {
        this->ntriangle = shape->ntriangle;
        this->vertex = shape->vertex;
        this->colors = shape->colors;
        this->model = mat4(1.0f);
        this->createVertexArray();
        this->nvertex = this->vertex.size();
    }

    // Constructs an object with a certain triangles number.
    Shape2D(int ntriangle)
    {
        this->ntriangle = ntriangle;
    }

    // Sets the color of the shape.
    void setColor(Color color) { this->color = color; }

    // Sets the middle vertex color.
    void setMidColor(Color mid) { this->midColor = color; }

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;

    virtual void clearShape() override;

    virtual void runAction() override {}

};
