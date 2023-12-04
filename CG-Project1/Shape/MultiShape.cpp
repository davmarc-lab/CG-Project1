#include "MultiShape.hpp"
#include "ComplexShape2D.hpp"
#include <glm/gtx/string_cast.hpp>

vec3 prePos;

void moveAnchor(ComplexShape2D* anchor, vec3 translateVector, vec3 scaleVector, vec3 rotationVector, float value)
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

    prePos = anchor->getPosition();
    auto result = anchor->getModelMatrix() * scale * rotate * translate;
    anchor->setModelMatrix(result);
}

void MultiShape::transformShapes(vec3 translateVector, vec3 scaleVector, vec3 rotationVector, float value)
{
    auto anchor = shapes[ANCHOR];
    moveAnchor(this->shapes[ANCHOR], translateVector, scaleVector, rotationVector, value);

    auto base = mat4(1.0f);
    auto rotVal = value;

    auto posAnchor = anchor->getPosition();
    for (int i = 1; i < shapes.size(); i++)
    {
        auto final = posAnchor - prePos;
        auto translate = glm::translate(base, final);
        auto scale = glm::scale(base, scaleVector);
        if (rotationVector == vec3(0))
        {
            rotationVector = vec3(1, 1, 1);
            rotVal = 0;
        }
        mat4 rotate = glm::rotate(base, radians(rotVal), rotationVector);

        auto result = shapes[i]->getModelMatrix() * scale * rotate * translate;
        result[3] = vec4(final + shapes[i]->getPosition(), 1);
        shapes[i]->setModelMatrix(result);
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
