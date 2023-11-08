#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include <vector>

class ComplexShape2D
{
public:
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;
    vector<vec3> vertex;
    vector<vec4> colors;
    Color color;
    Color midColor;
    mat4 model;
    int nvertex = 0;
    int ntriangle = 0;

public:

    // Add element to vertex vector3
    void addElementVertex(vec3 elem) { this->vertex.push_back(elem); }

    // Add element to colors vetcor4
    void addElementColors(vec4 elem) { this->colors.push_back(elem); }

    // Retrieves the vertex number of the shape
    int getVertexNum() { return this->nvertex; }

    void setVertexNum(int n) { this->nvertex = n; }

    // Retrieves the triangles nummber of the shape
    int getTriangleNum() { return this->ntriangle; }

    void setTriangleNum(int n) { this->ntriangle = n; }

    void setColor(Color color) { this->color = color; }

    void setMidColor(Color mid) { this->midColor = color; }

    vector<vec3> getVertexArray() { return this->vertex; }

    void setVertexArray(vector<vec3> vertex) { this->vertex = vertex; }

    void setColorsArray(vector<vec4> colors) { this->colors = colors; }

    mat4 getModelMatrix() { return this->model; }

    void setModelMatrix(mat4 model) { this->model = mat4(model); }
    
    // model matrix transoformation
    void scaleShape(vec3 mod) { this->setModelMatrix(scale(this->getModelMatrix(), mod)); }

    void translateShape(vec3 mod) { this->setModelMatrix(translate(this->getModelMatrix(), mod)); }

    void rotateShape(vec3 mod, float w) { this->setModelMatrix(rotate(this->getModelMatrix(), radians(w), mod)); }
    // end model matrix transoformation

    GLuint getVertexArrayObject() { return this->vao; }

    vector<vec4> getColorsArray() { return this->colors; }

    void clearVertexArray() { this->vertex.clear(); }

    void clearColorArray() { this->colors.clear(); }
 
    // Creates the VAO of the shape
    virtual void createVertexArray() = 0;

    // Create all vertex and color VBO, enable them and draw in the windows
    virtual void draw() = 0;
};
