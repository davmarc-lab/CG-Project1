#pragma once

#include "ComplexShape2D.hpp"
#include "../Structs.hpp"
#include <vector>

/*
 * This class creates a bullet implementing the ComplexShape2D interface.
 */
class Bullet : public ComplexShape2D
{
    private:
        // Bullets parts
        vector<ComplexShape2D*> shapes;

        // Bullet velocity
        float velocity = 1;

    public:
        // Constructs a bullet from a given position.
        Bullet(vec2 pos, vec4 baseColor, DoubleColor peakColor);

        Bullet();

        // Retrieves all the bullet elements.
        vector<ComplexShape2D*> getElements() { return this->shapes; }

        // Checks if the bullet is in collision with a given shape.
        bool checkShapesCollision(ComplexShape2D* shape);

        // Retrieves the top right corner of the bullet.
        vec2 getTopCorner();

        // Retrieves the bottom left corner of the bullet.
        vec2 getBotCorner();

        virtual void clearShape() override;

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void runAction() override;
};
