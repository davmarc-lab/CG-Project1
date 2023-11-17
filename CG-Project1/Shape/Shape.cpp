#include "Shape.hpp"
#include <glm/ext/matrix_clip_space.hpp>

using namespace std;

void Shape2D::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // VBO for geometric values
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

void Shape2D::draw(Shader shader) {
    GLuint modelLoc;

    modelLoc = glGetUniformLocation(shader.getId(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(this->getModelMatrix()));

    glBindVertexArray(this->getVertexArrayObject());
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->getVertexNum() + 2);
    glBindVertexArray(0);
}

void Shape2D::clearShape()
{
    this->clearVertexArray();
    this->clearColorArray();
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo_g);
    glDeleteBuffers(1, &this->vbo_c);
}
