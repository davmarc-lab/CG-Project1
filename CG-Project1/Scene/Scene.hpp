#pragma once

#include "../Shape/Shape.hpp"
#include "../Shader/Shader.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <utility>

using namespace std;

/*
 * This class creates a Scene that can be used to display ComplexShape2D object into the window.
 */
class Scene
{
private:
    // This vector store a ComplexShape2D and the corresponding Shader.
    vector<pair<ComplexShape2D*, Shader>> shapes;
    // It represent the porjection matrix used to draw each element of the Scene.
    mat4 projection;
    
public:
    // This constructor creates an empty scene.
    Scene() {}

    // This constructor create a scene with a given projection matrix.
    Scene(mat4 projection);

    // Add to the scene a given shape and his shader.
    void addShape2dToScene(ComplexShape2D* shape, Shader shader) { this->shapes.push_back(pair(shape, shader)); }

    // Retrieves all the element of the scene in a vector<pair<ComplexShape2D*, Shader>>.
    vector<pair<ComplexShape2D*, Shader>> getSceneElements() { return this->shapes; }

    // This method reshape the entire scene, should be followed by a modification of the projection.
    void reshape(int width, int height);

    // Set the projection matrix with a given matrix.
    void setProjectionMatrix(mat4 projection) { this->projection = mat4(projection); }

    // This method call all draw(shader) method of each shape implemented from ComplexShape2D class.
    void drawScene();
};
