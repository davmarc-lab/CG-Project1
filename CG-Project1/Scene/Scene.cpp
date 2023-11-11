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
        auto shape = this->getSceneElements()[i].first;
        auto shader = this->getSceneElements()[i].second;

        shader.use();
        GLuint projLoc = glGetUniformLocation(shader.getId(), "projection");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(this->projection));
        shape->draw(shader);
    }
}

void reshape(int width, int height)
{
    float aspectratio = (float) width / (float) height;

}
