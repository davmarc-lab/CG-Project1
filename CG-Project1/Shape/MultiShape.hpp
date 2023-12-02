#pragma once

#include "ComplexShape2D.hpp"
#include <vector>

class MultiShape : public ComplexShape2D
{
    private:
        vector<ComplexShape2D*> shapes;

    public:
        MultiShape() {}

        void addShape(ComplexShape2D* elem) { this->shapes.push_back(elem); }

        vector<ComplexShape2D*> getShapes() { return this->shapes; }

        void transformShapes(vec3 transalte, vec3 scale, vec3 rotate);

        virtual void createVertexArray() override;

        virtual void draw(Shader shader) override;

        virtual void runAction() override;

        virtual void clearShape() override;
};
