#include "Bullet.hpp"
#include "ComplexShape2D.hpp"
#include "Square.hpp"
#include "Shape.hpp"
#include "../Utils/utils.hpp"

Bullet::Bullet(vec2 pos)
{
    ComplexShape2D* base = new Square(color::BLACK);
    ComplexShape2D* peak = new Shape2D(50);
    base->createVertexArray();

    base->setModelMatrix(mat4(1.0f));
    base->translateShape(vec3(pos, 0));
    base->scaleShape(vec3(35, 25, 1));
    base->setSolid();

    peak->setColor(color::BLACK);
    peak->setMidColor(color::BLACK);
    Helper::buildCircle(0, 0, 1, 1, peak);
    peak->createVertexArray();

    peak->setModelMatrix(mat4(1.0f));
    peak->translateShape(vec3(pos.x - 30, pos.y, 0));
    peak->scaleShape(vec3(50, 25, 1));
    peak->setSolid();

    this->shapes.push_back(base);
    this->shapes.push_back(peak);

    this->velocity = (pow(sin(glfwGetTime()), 2) * (rand() % 4)) + 6;
}

void Bullet::runAction()
{
    for (auto elem: this->shapes)
    {
        auto pos = elem->getPosition();
        elem->setModelMatrix(mat4(1.0f));
        elem->setModelMatrix(translate(elem->getModelMatrix(), pos - vec3(velocity, 0, 0)));
        elem->scaleShape(vec3(25, 25, 1)); 
    }
}

void Bullet::clearShape()
{
    for (auto elem: this->shapes)
    {
        elem->clearShape();
    }
    this->shapes.clear();
}

void Bullet::createVertexArray()
{
    for (auto elem: this->shapes)
    {
        elem->createVertexArray();
    }
}

void Bullet::draw(Shader shader)
{
    for (auto elem: this->shapes)
    {
        elem->draw(shader);
    }
}
