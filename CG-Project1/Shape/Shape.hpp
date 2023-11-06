#pragma once

#include <vector>
#include "../Lib.hpp"
#include "../Color/Color.hpp"

class Shape2D
{
private:
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;
    vector<vec3> vertex;
    vector<vec4> colors;
    int nvertex = 0;
    int ntriangle = 0;
    mat4 model = mat4(1.0f);
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

    // Creates the VAO of the Shape2D
    void createVertexArray();

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

    Color getColor() { return this->color; }

    void setColor(vec4 color) { this->color = Color(color); }

    Color getMidColor() { return this->midColor; }

    void setMidColor(vec4 color) { this->midColor = Color(color); }

    vector<vec3> getVertexArray() { return this->vertex; }

    void setVertexArray(vector<vec3> vertex) { this->vertex = vertex; }

    void setColorsArray(vector<vec4> colors) { this->colors = colors; }

    mat4 getModelMatrix() { return this->model; }

    void setModelMatrix(mat4 model) { this->model = mat4(model); }

    GLuint getVertexArrayObject() { return this->vao; }

    vector<vec4> getColorsArray() { return this->colors; }

    void scaleShape(vec3 mod) { this->setModelMatrix(scale(this->getModelMatrix(), mod)); }

    void translateShape(vec3 mod) { this->setModelMatrix(translate(this->getModelMatrix(), mod)); }

    void rotateShape(vec3 mod, float w) { this->setModelMatrix(rotate(this->getModelMatrix(), radians(w), mod)); }
};
