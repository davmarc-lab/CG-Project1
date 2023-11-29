#pragma once

#include "../Lib.hpp"
#include "ComplexShape2D.hpp"

class Square : public ComplexShape2D
{
private:

	GLuint ebo;
	float vertices[12] = {
		-1.0f, -1.0f, 0.0f,		// bottom left
		-1.0f, 1.0f, 0.0f,		// top left
		1.0f, 1.0f, 0.0f,		// top right
		1.0f, -1.0f, 0.0f		// bottom right
	};
	unsigned int indices[6] = { 0, 1, 3, 1 ,2 ,3 };

public:
	Square(Color color) 
    {
        for (int i = 0; i < 12; i += 3)
            this->vertex.push_back(vec3(vertices[i], vertices[i+1], vertices[i+2]));

        for (int i = 0; i < 4; i++)
        {
            this->colors.push_back(color.getColorVector());
        }
        this->nvertex = 6;
    }

	virtual void draw(Shader shader) override;

	virtual void createVertexArray() override;

    virtual void clearShape() override;

    virtual void runAction() override {}

};
