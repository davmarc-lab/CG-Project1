#include "Game.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <thread>

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Window/Window.hpp"
#include "../Shape/ComplexShape2D.hpp"
#include "../Shape/Shape.hpp"
#include "../Shape/Square.hpp"
#include "../Shape/Curve.hpp"
#include "../Scene/Scene.hpp"
#include "../Utils/utils.hpp"

ComplexShape2D* enemy = new Shape2D(50);
ComplexShape2D* player = new Square(color::RED);
Shape2D gg = Shape2D(3);
vector<Helper> helpers;

Game::Game(unsigned int width, unsigned int height)
{
    this->state = GAME_NONE;
    this->width = width;
    this->height = height;
#define WIDTH width
#define HEIGHT height
#define ROADLIMIT height - 300

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
    glUniform2f(resLoc, this->width, ROADLIMIT);

    // Creates the shapes shader
    Shader shader("resources/vertexShader.vert", "resources/fragmentShader.frag");

    // Create all the shapes of the scene
    ComplexShape2D* road = new Square(color::WHITE);
    road->scaleShape(vec3(this->width, ROADLIMIT, 1));
    road->createVertexArray();

    player->setSolid();
    player->createVertexArray();
    player->translateShape(vec3(200, 200, 0));
    player->scaleShape(vec3(25, 25, 1));

    /* // real player shape */
    /* ComplexShape2D* carBody = new Square(color::RED); */
    /* carBody->createVertexArray(); */
    /* carBody->translateShape(vec3(160, 140, 0)); */
    /* carBody->scaleShape(vec3(94, 40, 1)); */

    /* ComplexShape2D* fwheel = new Shape2D(50); */
    /* fwheel->setColor(color::BLACK); */
    /* fwheel->setMidColor(color::WHITE); */
    /* Helper::buildCircle(0, 0, 1, 1, fwheel); */
    /* fwheel->createVertexArray(); */
    /* fwheel->translateShape(vec3(100, 100, 0)); */
    /* fwheel->scaleShape(vec3(20, 20, 1)); */

    /* ComplexShape2D* rwheel = new Shape2D(50); */
    /* rwheel->setColor(color::BLACK); */
    /* rwheel->setMidColor(color::WHITE); */
    /* Helper::buildCircle(0, 0, 1, 1, rwheel); */
    /* rwheel->createVertexArray(); */
    /* rwheel->translateShape(vec3(200, 100, 0)); */
    /* rwheel->scaleShape(vec3(20, 20, 1)); */

    enemy->setColor(color::BLACK);
    enemy->setMidColor(color::WHITE);
    Helper::buildCircle(0, 0, 1.0, 1.0, enemy);
    enemy->setSolid();
    enemy->createVertexArray();
    enemy->translateShape(vec3(1400, 200, 0));
    enemy->scaleShape(vec3(25, 25, 1));

    // Creates the drawing scenes with the projection matrix
    scene.addShape2dToScene(road, roadShader);
    scene.addShape2dToScene(player, shader);
    scene.addShape2dToScene(enemy, shader);

    srandom(20287462);

    for (int i = 0; i < 12; i++)
    {
        gg.setColor(color::WHITE);
        gg.setMidColor(color::WHITE);
        Helper::buildCircle(0, 0, 1, 1, &gg);
        gg.setSolid();
        gg.createVertexArray();
        auto pos = Helper::getRandomPosition2D(pair<int, int>(WIDTH, WIDTH), pair<int, int>(80, 500));
        gg.setModelMatrix(translate(mat4(1.0f), vec3(pos.x, pos.y, 0)));
        gg.scaleShape(vec3(25, 25, 1));
        scene.addShape2dToScene(new Shape2D((Shape2D)gg), shader, ShapeType::ENEMY);
        auto tmp = Helper(vec2(WIDTH, HEIGHT));
        tmp.setVelocity(((rand() % 9) + 6) * 0.03);
        helpers.push_back(tmp);
    }

    this->state = GAME_ACTIVE;

    /* bananaHelper.setYVelocity(0.05f); */
}

void Game::processInput(float deltaTime, Window window)
{
    if (this->state == GAME_ACTIVE)
    {
        float playerVelocity = 14 * deltaTime;
        vec3 playerPos = player->getPosition();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && (int)playerPos.y < ROADLIMIT - 50 - 25)
        {
            player->translateShape(vec3(0, playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS && playerPos.y > 50 + 25)
        {
            player->translateShape(vec3(0, -playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS && playerPos.x > 0 + 25)
        {
            player->translateShape(vec3(-playerVelocity, 0, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS && playerPos.x < this->width - 25)
        {
            player->translateShape(vec3(playerVelocity, 0, 0));
        }
    }
}

void Game::update(float deltaTime)
{

    int k = 0;
    for (int i = 0; i < scene.getSceneElements().size(); i++)
    {
        auto elem = scene.getSceneElements()[i];
        if (elem.type != ShapeType::ENEMY)
            continue;

        helpers[k].enemyMoveAction(vec3(-1, 0, 0), elem.shape);
        /* cout << to_string(elem.shape->getPosition()) << endl; */
        k++;

        if (player->checkCollision(elem.shape))
        {
            player->setDestroyed();
        }
    }

    if (player->checkCollision(enemy) || player->checkCollision(&gg))
    {
        player->setDestroyed();
    }
}

void Game::render()
{
    scene.drawScene();
}

void Game::clear()
{
    scene.clear();
}

