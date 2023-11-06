#pragma once

#include "../Lib.hpp"
#include "../Shape/Shape.hpp"
#include <vector>

class ComplexShape2D
{
private:
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;
    GLuint ebo;
    vector<vec3> vertex;
    vector<vec4> colors;
    vector<vec3> indexes;
    mat4 model;
    int fragmentType;
    int nvertex = 0;
    int ntriangle = 0;

public:
    vector<vec3> CP;
    vector<vec4> colCP;

    ComplexShape2D(int ntriangle, vector<vec3> vertex,
                   vector<vec4> colors, vector<vec3> indexes, GLuint ebo, mat4 model,
                   int fragmentType);

    ComplexShape2D(int ntriangle);

    // Creates the VAO of the shape
    void createVertexArray();

    // Create all vertex and color VBO, enable them and draw in the windows
    void drawShape2D(int n);

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

    vector<vec3> getVertexArray() { return this->vertex; }

    void setVertexArray(vector<vec3> vertex) { this->vertex = vertex; }

    void setColorsArray(vector<vec4> colors) { this->colors = colors; }

    mat4 getModelMatrix() { return this->model; }

    void setModelMatrix(mat4 model) { this->model = mat4(model); }

    GLuint getVertexArrayObject() { return this->vao; }

    vector<vec4> getColorsArray() { return this->colors; }

    void addControlPoints(vec3 elem) { this->indexes.push_back(elem); }

    vector<vec3> getIndexArray() { return this->indexes; }

    int getFragmentType() { return this->fragmentType; }

    void setFragmentType(int value) { this->fragmentType = value; }

    void clearVertexArray() { this->vertex.clear(); }

    void clearColorArray() { this->colors.clear(); }
};
