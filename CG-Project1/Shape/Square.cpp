#include "Square.hpp"

void Square::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo_g);
    glGenBuffers(1, &this->vbo_c);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertex), this->vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
}

void Square::draw() 
{   
    GLuint modelLoc, projLoc;
    mat4 projection = ortho(0.0f, 1600.0f, 0.0f, 900.0f);

    this->useShader();

    this->setModelMatrix(mat4(1.0f));
    this->translateShape(vec3(0, 0, 0));
    this->scaleShape(vec3(1600, 900 / 2, 1));

    modelLoc = glGetUniformLocation(this->shader.getId(), "model");
    projLoc = glGetUniformLocation(this->shader.getId(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(this->getModelMatrix()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
    glBindVertexArray(this->getVertexArrayObject());
    glDrawElements(GL_TRIANGLES, this->getVertexNum(), GL_UNSIGNED_INT, 0);
}
