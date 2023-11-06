#include "Scene.hpp"

#include <iostream>

Scene::Scene()
{
}

// vector<Shape2D> Scene::getSceneShapes2d()
// {
//     vector<Shape2D> vecs;
//     for (int i = 0; i < this->shapes2D.size(); i++)
//     {
//         vecs.push_back(this->shapes2D[i].first);
//     }
//     return vecs;
// }

void Scene::drawScene(GLuint id)
{
    for (int i = 0; i < this->getSceneElements().size(); i++)
    {
        Shape2D shape = this->getSceneElements()[i].first;
        GLenum renderMode = this->getSceneElements()[i].second;

        if (i == 0)
        {
            shape.setModelMatrix(mat4(1.0f));
            shape.setModelMatrix(translate(shape.getModelMatrix(), vec3(400, 300, 0.0)));
            shape.setModelMatrix(scale(shape.getModelMatrix(), vec3(300, 300, 1.0)));
        }
        else
        {
            shape.setModelMatrix(mat4(1.0f));
            shape.setModelMatrix(translate(shape.getModelMatrix(), vec3(1000, 300, 0.0)));
            shape.setModelMatrix(scale(shape.getModelMatrix(), vec3(600, 600, 1.0)));
        }
        GLuint modelLoc = glGetUniformLocation(id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(shape.getModelMatrix()));

        glBindVertexArray(shape.getVertexArrayObject());
        glDrawArrays(renderMode, 0, shape.getVertexNum());
    }
}

void reshape(int width, int height)
{
    float aspectratio = (float) width / (float) height;

    // if (aspectratio < 
}
