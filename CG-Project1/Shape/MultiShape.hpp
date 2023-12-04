#pragma once

#include "../Lib.hpp"

#include "ComplexShape2D.hpp"

#define ANCHOR 0

class MultiShape : public ComplexShape2D
{
    private:
        vector<ComplexShape2D*> shapes;

    public:
        MultiShape() {}

        void addShape(ComplexShape2D* elem) { this->shapes.push_back(elem); }

        vector<ComplexShape2D*> getShapes() { return this->shapes; }

        void transformShapes(vec3 translateVector, vec3 scaleVector, vec3 rotationVector, float value);

        pair<vec3, vec3> getBoundingBox();

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void runAction() override;

        virtual void clearShape() override;
};
