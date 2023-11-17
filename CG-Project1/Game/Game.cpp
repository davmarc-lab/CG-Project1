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

mat4 projection = ortho(0.0f, (float)1600, 0.0f, (float)900);

Scene scene = Scene(projection);
ComplexShape2D* player = new Shape2D(50);
/* Helper enemHelper = Helper(window.getResolution()); */
/* Helper bananaHelper = Helper(window.getResolution()); */

Game::Game(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
}

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
    Curve* herm = new Curve();
    herm->readDataFromFile("resources/hermite/boomerang.txt");
    herm->buildHermite(color::YELLOW, color::WHITE, herm);

    herm->createVertexArray();
    herm->translateShape(vec3(500, 200, 0));
    herm->scaleShape(vec3(150, 150, 1));

    // Creates the drawing scenes with the projection matrix
    scene.addShape2dToScene(c, roadShader);
    scene.addShape2dToScene(player, shader);
    scene.addShape2dToScene(enemy, shader);
    scene.addShape2dToScene(herm, shader);

    /* bananaHelper.setYVelocity(0.05f); */
}

void Game::processInput(float deltaTime)
{

}

void Game::update(float deltaTime)
{

}

void Game::render()
{
    scene.drawScene();
}

