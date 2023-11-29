#pragma once

#include "Action.hpp"
#include "ComplexShape2D.hpp"
#include <vector>
class Bullet : public ComplexShape2D
{
    private:
        vector<ComplexShape2D*> shapes;

        float velocity = 1;

    public:
        Bullet(vec2 pos);

        Bullet();

        vector<ComplexShape2D*> getElements() { return this->shapes; }

        bool checkShapesCollision(ComplexShape2D* shape);

        vec2 getTopCorner();

        vec2 getBotCorner();

        virtual void clearShape() override;

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void runAction() override;
};
