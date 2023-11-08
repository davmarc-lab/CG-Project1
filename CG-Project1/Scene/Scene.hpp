#pragma once

#include "../Shape/Shape.hpp"
#include "../Shader/Shader.hpp"
#include <iostream>
#include <utility>

using namespace std;

class Scene
{
private:
    vector<ComplexShape2D*> shapes;
public:
    Scene();

    void addShape2dToScene(ComplexShape2D* shape, Shader shader) { this->shapes.push_back(shape); }

    vector<ComplexShape2D*> getSceneElements() { return this->shapes; }

    GLenum getRenderMode(ComplexShape2D shape);

    void reshape(int width, int height);

    void drawScene();
};
