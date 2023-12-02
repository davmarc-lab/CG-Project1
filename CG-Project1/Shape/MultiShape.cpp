#include "MultiShape.hpp"

void MultiShape::transformShapes(vec3 translate, vec3 scale, vec3 rotate)
{
    for (auto elem: this->shapes)
    {
        /* elem->setModelMatrix(mat4(1.0f)); */
        elem->translateShape(translate);
        elem->scaleShape(scale);
        /* elem->rotateShape(vec3(0, 0, 1), 10); */
    }
}

void MultiShape::createVertexArray()
{
    for (auto elem: this->shapes)
    {
        elem->createVertexArray();
    }
}

void MultiShape::draw(Shader shader)
{
    for (auto elem: this->shapes)
    {
        /* shader.use(); */
        elem->draw(shader);
    }
}

void MultiShape::runAction()
{
    for (auto elem: this->shapes)
    {

    }
}

void MultiShape::clearShape()
{
    for (auto elem: this->shapes)
    {
        elem->clearShape();
    }

    this->shapes.clear();
}
