#include "ComplexShape2D.hpp"

ComplexShape2D::ComplexShape2D(int ntriangle, vector<vec3> vertex,
                               vector<vec4> colors, vector<vec3> indexes, GLuint ebo, mat4 model,
                               int fragmentType)
{
    this->ntriangle = ntriangle;
    this->vertex = vertex;
    this->colors = colors;
    this->model = model;
    this->nvertex = this->vertex.size();
    this->ebo = ebo;
    this->indexes = indexes;
    this->fragmentType = fragmentType;
}

ComplexShape2D::ComplexShape2D(int ntriangle)
{
    this->ntriangle = ntriangle;
}

void ComplexShape2D::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(vec3), this->vertex.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // VBO for color values
    glGenBuffers(1, &this->vbo_c);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
}
