#include "Scene.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

Scene::Scene()
{
}

void Scene::drawScene()
{
    for (int i = 0; i < this->getSceneElements().size(); i++)
    {
        this->getSceneElements()[i]->draw();
    }
}

void reshape(int width, int height)
{
    float aspectratio = (float) width / (float) height;

    // if (aspectratio < 
}
