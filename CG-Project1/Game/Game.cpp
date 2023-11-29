#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/common.hpp>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Window/Window.hpp"
#include "../Shape/ComplexShape2D.hpp"
#include "../Shape/Shape.hpp"
#include "../Shape/Square.hpp"
#include "../Shape/Curve.hpp"
#include "../Shape/Bullet.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/TextScene.hpp"
#include "../Utils/utils.hpp"

struct Player
{
    ComplexShape2D* shape;
    unsigned int hp = 3;
    int ammo = 0;
} player;

Curve* goal;
vector<ComplexShape2D*> enemies;

vector<Helper> helpers;

string textAmmoPrefix = "Ammo: ";

unsigned int gameLevel = 0;

Game::Game(unsigned int width, unsigned int height)
{
    this->state = GAME_NONE;
    this->width = width;
    this->height = height;
#define WIDTH width
#define HEIGHT height
#define ROADLIMIT height - 300

    srand(time(NULL));
}

mat4 projection = ortho(0.0f, (float)1600, 0.0f, (float)900);

Scene shapeScene = Scene(projection);
TextScene textScene = TextScene(projection);
/* Helper enemHelper = Helper(window.getResolution()); */
/* Helper boomerangHelper = Helper(window.getResolution()); */

ComplexShape2D* bullet;

void buildBullet(vec2 pos)
{
    bullet = new Bullet(pos);
    bullet->createVertexArray();

    Shader shader("resources/vertexShader.vert", "resources/fragmentShader.frag");
    shapeScene.addShape2dToScene(bullet, shader);
}

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
    goal = new Curve();
    player.shape = new Square(color::RED);

    ComplexShape2D* road = new Square(color::WHITE);
    road->scaleShape(vec3(this->width, ROADLIMIT, 1));
    road->createVertexArray();

    player.shape->createVertexArray();
    player.shape->translateShape(vec3(200, 200, 0));
    player.shape->scaleShape(vec3(25, 25, 1));
    player.shape->setSolid();

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

    goal->readDataFromFile("./resources/hermite/bullet.txt");
    goal->buildHermite(color::YELLOW, color::YELLOW);
    goal->createVertexArray();
    goal->translateShape(vec3(400, 200, 0));
    goal->scaleShape(vec3(25, 25, 1));
    goal->setSolid();



    // Creates the drawing scenes with the projection matrix
    shapeScene.addShape2dToScene(road, roadShader);
    shapeScene.addShape2dToScene(goal, shader);
    shapeScene.addShape2dToScene(player.shape, shader);

    for (int i = 0; i < gameLevel; i++)
    {
        auto pos = Helper::getRandomPosition2D(pair<int, int>(WIDTH, WIDTH), pair<int, int>(80, 500));

        buildBullet(pos);
    }

    // Initialize texts in the window
    Text textLevel = Text(projection, "Level: ", 60);
    textLevel.setPosition(vec2(40, 820));
    textLevel.initializeTextRender();
    textLevel.createVertexArray();

    textLevel.appendText(to_string(gameLevel));

    Text textAmmo = Text(projection, textAmmoPrefix, 60);
    textAmmo.setPosition(vec2(1300, 820));
    textAmmo.initializeTextRender();
    textAmmo.createVertexArray();

    textAmmo.appendText(to_string(player.ammo));

    Shader textShader = Shader("./resources/textVertexShader.vert", "./resources/textFragmentShader.frag");

    textScene.addTextToScene(textLevel, textShader);
    textScene.addTextToScene(textAmmo, textShader);

    this->state = GAME_ACTIVE;
    gameLevel++;

    glDeleteShader(shader.getId());
    glDeleteShader(roadShader.getId());
}

void playerShoot(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        player.ammo--;
        string result = textAmmoPrefix;
        result.append(to_string(player.ammo));
        textScene.getSceneElements()[1].first.setText(result);
        cout << "SHOOT DUDE!" << endl;
    }
}

void Game::processInput(float deltaTime, Window window)
{
    if (this->state == GAME_ACTIVE)
    {
        float playerVelocity = 14 * deltaTime;
        vec3 playerPos = player.shape->getPosition();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && (int)playerPos.y < ROADLIMIT - 50 - 25)
        {
            player.shape->translateShape(vec3(0, playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS && playerPos.y > 50 + 25)
        {
            player.shape->translateShape(vec3(0, -playerVelocity, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS && playerPos.x > 0 + 25)
        {
            player.shape->translateShape(vec3(-playerVelocity, 0, 0));
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS && playerPos.x < this->width - 25)
        {
            player.shape->translateShape(vec3(playerVelocity, 0, 0));
        }

        glfwSetKeyCallback(window.getWindow(), playerShoot);

    }
}

void Game::update(float deltaTime)
{

    for (auto elem: shapeScene.getSceneElements())
    {
        elem.shape->runAction();
    }

    // checks if player reach the goal
    if (player.shape->checkCollision(goal))
    {
        player.ammo++;
        this->clear();
        this->init();
        this->render();
    }
}

void Game::render()
{
    shapeScene.drawScene();
    textScene.drawScene();
}

void Game::clear()
{
    shapeScene.clear();
    helpers.clear();
    textScene.clear();
}

