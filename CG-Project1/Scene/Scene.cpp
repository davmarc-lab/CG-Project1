#include "Scene.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

Scene::Scene()
{
}

void Scene::drawScene()
{
    for (int i = 0; i < this->getSceneElements().size(); i++)
    {
        this->getSceneElements()[i].first->draw(this->getSceneElements()[i].second);
        /* cout << to_string(this->getSceneElements()[i].first->getModelMatrix()) << endl; */
    }
}

void reshape(int width, int height)
{
    float aspectratio = (float) width / (float) height;

    // if (aspectratio < 
}
