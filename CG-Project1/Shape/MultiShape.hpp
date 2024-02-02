#pragma once

#include "../Lib.hpp"

#include "ComplexShape2D.hpp"

#define ANCHOR 0

/*
 * This class creates a multi entity shape, defining the first items like an anchor.
 */
class MultiShape : public ComplexShape2D
{
    private:
        vector<ComplexShape2D*> shapes;

    public:
        // Construct an empty shape.
        MultiShape() {}

        // Adds entities to the shape.
        void addShape(ComplexShape2D* elem) { this->shapes.push_back(elem); }

        // Retrieves all the entities.
        vector<ComplexShape2D*> getShapes() { return this->shapes; }

        // Transform all the entities of the shape.
        void transformShapes(vec3 translateVector, vec3 scaleVector, vec3 rotationVector, float value);

        // Retrieves world coordinates of the total bounding box.
        pair<vec3, vec3> getBoundingBox();

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void runAction() override;

        virtual void clearShape() override;
};
