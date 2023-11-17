#include "Game.hpp"

#include <iostream>

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Window/Window.hpp"
#include "../Shape/ComplexShape2D.hpp"
#include "../Shape/Shape.hpp"
#include "../Shape/Square.hpp"
#include "../Shape/Curve.hpp"
#include "../Scene/Scene.hpp"
#include "../Utils/utils.hpp"

ComplexShape2D* player = new Shape2D(50);
Curve* boomer = new Curve();

Game::Game(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
#define WIDTH width
#define HEIGHT height
}

mat4 projection = ortho(0.0f, (float)1600, 0.0f, (float)900);

Scene scene = Scene(projection);
/* Helper enemHelper = Helper(window.getResolution()); */
/* Helper boomerangHelper = Helper(window.getResolution()); */

void Game::init()
{
    // Create the background shader and set the window resolution for drawing purpose
    Shader roadShader("resources/vertexShader.vert", "resources/backFragShader.frag");
    roadShader.use();
    GLuint resLoc = glGetUniformLocation(roadShader.getId(), "resolution");
    glUniform2f(resLoc, this->width, this->height);

    // Creates the shapes shader
    Shader shader("resources/vertexShader.vert", "resources/fragmentShader.frag");

    // Create all the shapes of the scene
    ComplexShape2D* c = new Square();
    c->scaleShape(vec3(this->width, this->height / 2, 1));
    c->createVertexArray();

    player->setColor(color::WHITE);
    player->setMidColor(color::RED);
    Helper::buildCircle(0, 0, 1.0, 1.0, player);
    player->setCollision(projection);
    player->createVertexArray();

    player->translateShape(vec3(200, 200, 0));
    player->scaleShape(vec3(25, 25, 1));

    ComplexShape2D* enemy = new Shape2D(50);
    enemy->setColor(color::BLACK);
    enemy->setMidColor(color::WHITE);
    Helper::buildCircle(0, 0, 1.0, 1.0, enemy);
    enemy->createVertexArray();
    enemy->translateShape(vec3(1400, 200, 0));
    enemy->scaleShape(vec3(25, 25, 1));

    // Create a shape from an hermite curve file
    boomer->readDataFromFile("resources/hermite/boomerang.txt");
    boomer->buildHermite(color::BLUE, color::WHITE, boomer);

    boomer->createVertexArray();
    boomer->translateShape(vec3(500, 200, 0));
    boomer->scaleShape(vec3(150, 150, 1));

    // Creates the drawing scenes with the projection matrix
    scene.addShape2dToScene(c, roadShader);
    scene.addShape2dToScene(player, shader);
    scene.addShape2dToScene(enemy, shader);
    scene.addShape2dToScene(boomer, shader);

    this->state = GAME_ACTIVE;

    /* bananaHelper.setYVelocity(0.05f); */
}

void Game::processInput(float deltaTime, Window window)
{
    if (this->state == GAME_ACTIVE)
    {
        float playerVelocity = 13 * deltaTime;
        vec3 playerPos = player->getPosition();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && playerPos.y < (float)this->height / 2 - 50 - 25)
        {
            player->translateShape(vec3(0, playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS && playerPos.y > 0 + 50 + 25)
        {
            player->translateShape(vec3(0, -playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS && playerPos.x > 0 + 25)
        {
            player->translateShape(vec3(-playerVelocity, 0, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS && playerPos.x < this->width - 27)
        {
            player->translateShape(vec3(playerVelocity, 0, 0));
        }

    }
}

void rotateObject(vec3 rotation, ComplexShape2D* shape, float deltaTime)
{
    float velocity = 600 * deltaTime;
    shape->rotateShape(rotation, velocity);
}

void Game::update(float deltaTime)
{
    rotateObject(vec3(0, 0, 1), boomer, deltaTime);
}

void Game::render()
{
    scene.drawScene();
}

void Game::clear()
{
    scene.clear();
}

