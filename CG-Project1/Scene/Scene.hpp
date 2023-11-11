#pragma once

#include "../Shape/Shape.hpp"
#include "../Shader/Shader.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <utility>

using namespace std;

class Scene
{
private:
    vector<pair<ComplexShape2D*, Shader>> shapes;
    mat4 projection;
    
public:
    Scene() {}

    Scene(mat4 projection);

    void addShape2dToScene(ComplexShape2D* shape, Shader shader) { this->shapes.push_back(pair(shape, shader)); }

    vector<pair<ComplexShape2D*, Shader>> getSceneElements() { return this->shapes; }

    GLenum getRenderMode(ComplexShape2D shape);

    void reshape(int width, int height);

    void drawScene();
};
