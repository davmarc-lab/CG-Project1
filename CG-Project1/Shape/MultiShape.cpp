#include "MultiShape.hpp"
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <mutex>

void MultiShape::transformShapes(vec3 translateVector, vec3 scaleVector, vec3 rotationVector, float value)
{
    for (auto elem: this->shapes)
    {
        auto rotVal = value;
        auto base = mat4(1.0f);

        auto translate = glm::translate(base, value * translateVector);
        auto scale = glm::scale(base, scaleVector);
        if (rotationVector == vec3(0))
        {
            rotationVector = vec3(1, 1, 1);
            rotVal = 0;
        }
        mat4 rotate = glm::rotate(base, radians(rotVal), rotationVector);

        auto result = elem->getModelMatrix() * scale * rotate * translate;
        elem->setModelMatrix(result);
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
