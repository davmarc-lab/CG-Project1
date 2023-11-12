#pragma once

#include "../Lib.hpp"
#include "ComplexShape2D.hpp"
#include <vector>

class Curve : public ComplexShape2D
{
public:
    vector<vec3> CP;
    vector<vec4> colCP;

public:

    virtual void createVertexArray() override;

    virtual void draw(Shader shader) override;
};
