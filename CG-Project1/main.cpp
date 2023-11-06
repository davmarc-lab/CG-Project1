﻿#include "Lib.hpp"
#include "Window/Window.hpp"
#include "Shape/Shape.hpp"
#include "Shape/Square.hpp"
#include "Shader/Shader.hpp"
#include "Scene/Scene.hpp"
#include "Color/Color.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#define PI 3.14159265358979323846

const int WIDTH = 1600, HEIGHT = 900;
GLuint modelLoc;

void initWindowView(Shader shader)
{
	mat4 view = translate(view, vec3(0.0f, 0.0f, -3));
	mat4 projection = ortho(0.0f, (float)WIDTH, 0.0f, float(HEIGHT));

	GLuint viewLoc = glGetUniformLocation(shader.getId(), "view");
	GLuint modelLoc = glGetUniformLocation(shader.getId(), "model");
	GLuint projLoc = glGetUniformLocation(shader.getId(), "projection");

	// glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
}

void buildCircle(float cx, float cy, float raggiox, float raggioy, Shape2D* fig)
{
	float stepA = (2 * PI) / fig->getTriangleNum();
	float t, xx, yy;

	fig->addElementVertex(vec3(cx, cy, 0.0));

	fig->addElementColors(fig->getMidColor().getColorVector());

	for (int i = 0; i <= fig->getTriangleNum(); i++)
	{
		t = (float)i * stepA;
		xx = cx + raggiox * cos(t);
		yy = cy + raggioy * sin(t);
		fig->addElementVertex(vec3(xx, yy, 0.0));
		// Colore
		fig->addElementColors(fig->getColor().getColorVector()); // Nota che la quarta componente corrisponde alla trasparenza del colore
	}

	fig->setVertexNum(fig->getTriangleNum());
}

mat4 projection = ortho(0.0f, (float)WIDTH, 0.0f, float(HEIGHT));

bool checkOutOfBounds(vec3 pos)
{
	cout << pos.x << ", " << pos.y << endl;
	if (pos.x > WIDTH || pos.x < 0 || pos.y > 500 || pos.y < 0)
		return true;
	return false;
}

// keep the player in the box
void processPlayerInput(Window window, Shape2D* player)
{
	float velocity = 0.2;
	int pixel = 1;

	vec3 pos = vec3(player->getModelMatrix()[3]);

	if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && pos.y < 500 - 50 - 25)
	{
		player->translateShape(vec3(0, pixel * velocity, 0));
	}

	if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS && pos.y > 0 + 50 + 25)
	{
		player->translateShape(vec3(0, -pixel * velocity, 0));
	}

	if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS && pos.x > 0 + 25)
	{
		player->translateShape(vec3(-pixel * velocity, 0, 0));
	}

	if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS && pos.x < WIDTH - 27)
	{
		player->translateShape(vec3(pixel * velocity, 0, 0));
	}
	// cout << to_string(player->getModelMatrix()) << endl;
	// cout << pos.x << ", " << pos.y << endl;
}

int startDirection = 1;

bool checkInRoadBound(float y)
{
	return y < 500 - 50 - 25 && y > 0 + 50 + 25;
}

void enemyMoveAction(Shape2D* entity)
{
	float velocity = 0.1;
	int pixel = 1;
	vec3 pos = vec3(entity->getModelMatrix()[3]);

	if (!checkInRoadBound(pos.y))
		startDirection *= -1;
	entity->translateShape(vec3(0, startDirection * pixel * velocity, 0));

	// cout << pos.x << ", " << pos.y << endl;
}

int main()
{
	Window w = Window("Hello rect", WIDTH, HEIGHT);
	if (w.initializeWindow() == 0)
	{
		Shader roadShader("resources/vertexShader.vert", "resources/roadFragShader.frag");
		Shader shader("resources/vertexShader.vert", "resources/fragmentShader.frag");

		Square c = Square();
		c.createVertexArray();

		Shape2D player = Shape2D(50);
		player.setColor(color::WHITE);
		player.setMidColor(color::RED);
		buildCircle(0, 0, 1.0, 1.0, &player);
		player.createVertexArray();

		player.setModelMatrix(mat4(1.0f));
		player.translateShape(vec3(200, 200, 0));
		player.scaleShape(vec3(25, 25, 1));

		Shape2D enemy = Shape2D(50);
		enemy.setColor(color::BLACK);
		enemy.setMidColor(color::WHITE);
		buildCircle(0, 0, 1.0, 1.0, &enemy);
		enemy.createVertexArray();

		enemy.setModelMatrix(mat4(1.0f));
		enemy.translateShape(vec3(1400, 200, 0));
		enemy.scaleShape(vec3(25, 25, 1));

		initWindowView(shader);
		GLuint roadLoc = glGetUniformLocation(shader.getId(), "road");

		while (!glfwWindowShouldClose(w.getWindow()))
		{
			GLuint modelLoc;
			GLuint projLoc;

			// input
			w.processCloseInput();
			processPlayerInput(w, &player);

			// render
			glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			roadShader.use();
			c.setModelMatrix(mat4(1.0f));
			c.translateShape(vec3(0, 0, 0));
			c.scaleShape(vec3(WIDTH, HEIGHT / 2, 1));

			modelLoc = glGetUniformLocation(roadShader.getId(), "model");
			projLoc = glGetUniformLocation(roadShader.getId(), "projection");

			glUniform1i(roadLoc, 1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(c.getModelMatrix()));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
			glBindVertexArray(c.getVertexArrayObject());
			glDrawElements(GL_TRIANGLES, c.getVertexNum(), GL_UNSIGNED_INT, 0);

			shader.use();

			modelLoc = glGetUniformLocation(shader.getId(), "model");
			projLoc = glGetUniformLocation(shader.getId(), "projection");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(player.getModelMatrix()));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

			glBindVertexArray(player.getVertexArrayObject());
			glDrawArrays(GL_TRIANGLE_FAN, 0, player.getVertexNum() + 2);

			// swap buffers and poll IO events
			glfwSwapBuffers(w.getWindow());
			glfwPollEvents();
		}
		w.terminateWindow();
	}
	else
	{
		cout << "Cannot start the application, due to GLFW error" << endl;
		return -1;
	}

	return 0;
}
