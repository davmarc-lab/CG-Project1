#include "Bullet.hpp"
#include "ComplexShape2D.hpp"
#include "Square.hpp"
#include "Shape.hpp"
#include "../Utils/utils.hpp"
#include <glm/gtx/string_cast.hpp>

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

vec2 Bullet::getTopCorner()
{
    float xmax = -1, ymax = -1;
    vec4 point;

    for (auto elem: this->shapes)
    {
        // all normalized points!
        auto points = elem->getVertexArray();

        for (int i = 0; i < points.size(); i++)
            xmax = xmax <= points[i].x ? points[i].x : xmax;
        for (int i = 0; i < points.size(); i++)
            ymax = ymax <= points[i].y ? points[i].y : ymax;

        point = elem->getModelMatrix() * vec4(xmax, ymax, 0, 1);
    }

    return vec2(point.x, point.y);
}

vec2 Bullet::getBotCorner()
{
    float xmin = 1, ymin = 1;
    vec4 point;

    for (auto elem: this->shapes)
    {
        // all normalized points!
        auto points = elem->getVertexArray();

        for (int i = 0; i < points.size(); i++)
            xmin = xmin >= points[i].x ? points[i].x : xmin;
        for (int i = 0; i < points.size(); i++)
            ymin = ymin >= points[i].y ? points[i].y : ymin;

        point = elem->getModelMatrix() * vec4(xmin, ymin, 0, 1);
    }

    return vec2(point.x, point.y);
}

bool Bullet::checkShapesCollision(ComplexShape2D* shape)
{
    if (shape->isSolid)
    {
        bool collisionX = false, collisionY = false;
        auto firstTopPos = getTopCorner();
        auto firstBotPos = getBotCorner();

        auto top = shape->getModelMatrix() * vec4(1, 1, 0, 1);
        auto secondTopPos = vec2(top.x, top.y);
        auto bot = shape->getModelMatrix() * vec4(-1, -1, 0, 1);
        auto secondBotPos = vec2(bot.x, bot.y);
        
        collisionX = firstBotPos.x <= secondTopPos.x && firstTopPos.x >= secondBotPos.x;
        collisionY = firstBotPos.y <= secondTopPos.y && firstTopPos.y >= secondBotPos.y;

        return collisionX && collisionY;
    }

    return false;
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
