#pragma once

#include "../Lib.hpp"
#include <vector>

class Square
{
private:

	GLuint vao;
	GLuint vbo_g;
	GLuint vbo_c;
	GLuint ebo;
	float vertex[12] = {
		1.0f, 1.0f, 0.0f,		// top right
		1.0f, -1.0f, 0.0f,		// bottom right
		-1.0f, -1.0f, 0.0f,		// bottom left
		-1.0f, 1.0f, 0.0f		// top left
	};
	unsigned int indices[6] = { 0, 1, 3, 1 ,2 ,3 };
	vector<vec4> colors;
	mat4 model = mat4(1.0f);
	int nvertex = 6;

public:
	Square() {}

	void draw();

	GLuint getVertexArrayObject() { return this->vao; }

	void createVertexArray();

	int getVertexNum() { return this->nvertex; }

	mat4 getModelMatrix() { return this->model; }

	void setModelMatrix(mat4 model) { this->model = mat4(model); }

	// modify model matrix
	void scaleShape(vec3 vec) { this->setModelMatrix(scale(this->getModelMatrix(), vec)); }

	void translateShape(vec3 vec) { this->setModelMatrix(translate(this->getModelMatrix(), vec)); }

	void rotateShape(vec3 vec, float w) { this->setModelMatrix(rotate(this->getModelMatrix(), radians(w), vec)); }
};