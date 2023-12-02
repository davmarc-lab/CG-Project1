#include "Scene.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

Scene::Scene(mat4 projection)
{
    this->projection = mat4(projection);
}

void Scene::drawScene()
{
    for (int i = 0; i < this->getSceneElements().size(); i++)
    {
        auto shape = this->getSceneElements()[i].shape;
        auto shader = this->getSceneElements()[i].shader;

        if (shape->isAlive())
        {
            shader.use();
            GLuint projLoc = glGetUniformLocation(shader.getId(), "projection");

            glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(this->projection));
            shape->draw(shader);
        }
    }
}

// DOESNT WORK
void Scene::removeElement(int index)
{
    this->shapes.erase(this->shapes.begin() + index);
}

void Scene::clear()
{
    for (auto elem: this->getSceneElements())
    {
        elem.shape->clearShape();
        elem.shader.clearShader();
    }
    this->shapes.clear();
}
