#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
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
#include "../Shape/MultiShape.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/TextScene.hpp"
#include "../Utils/utils.hpp"

// constant used to indicate the x and y tollerance out of the screen shapes position.
#define TOL 500

int wHeight, wWidth;

struct Player
{
    MultiShape* car = new MultiShape();
    unsigned int hp = 3;
    int ammo = 1;
} player;

Curve* goal;
vector<ComplexShape2D*> enemies;

string textAmmoPrefix = "Ammo: ";

unsigned int gameLevel = 1;

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

Bullet* bullet;

void buildBullet(vec2 pos)
{
    bullet = new Bullet(pos);
    bullet->createVertexArray();

    Shader shader("resources/vertexShader.glsl", "resources/fragmentShader.glsl");
    shapeScene.addShape2dToScene(bullet, shader, ShapeType::BULLET);
}

Shader roadShader;
GLuint timeLoc;

void Game::initGame()
{
    // Create the background shader and set the window resolution for drawing purpose
    roadShader = Shader("resources/vertexShader.glsl", "resources/backFragShader.glsl");
    roadShader.use();
    GLuint resLoc = glGetUniformLocation(roadShader.getId(), "resolution");
    glUniform2f(resLoc, this->width, ROADLIMIT);
    timeLoc = glGetUniformLocation(roadShader.getId(), "iTime");
    glUniform1f(timeLoc, glfwGetTime());

    // Creates the shapes shader
    Shader shader("resources/vertexShader.glsl", "resources/fragmentShader.glsl");

    // Create all the shapes of the scene
    goal = new Curve();

    ComplexShape2D* road = new Square(color::WHITE);
    road->scaleShape(vec3(this->width, this->height, 1));
    road->createVertexArray();

    // real player shape
    ComplexShape2D* carBody = new Square(color::RED);
    carBody->createVertexArray();
    carBody->translateShape(vec3(153, 140, 0));
    carBody->scaleShape(vec3(89, 40, 1));
    carBody->setSolid();

    ComplexShape2D* fwheel = new Shape2D(50);
    fwheel->setColor(color::BLACK);
    fwheel->setMidColor(color::WHITE);
    Helper::buildCircle(0, 0, 1, 1, fwheel);
    fwheel->createVertexArray();
    fwheel->translateShape(vec3(200, 100, 0));
    fwheel->scaleShape(vec3(20, 20, 1));
    fwheel->setSolid();

    ComplexShape2D* rwheel = new Shape2D(50);
    rwheel->setColor(color::BLACK);
    rwheel->setMidColor(color::WHITE);
    Helper::buildCircle(0, 0, 1, 1, rwheel);
    rwheel->createVertexArray();
    rwheel->translateShape(vec3(100, 100, 0));
    rwheel->scaleShape(vec3(20, 20, 1));
    rwheel->setSolid();

    ComplexShape2D* carWindow = new Square(Color(0.78f, 0.96f, 0.94f, 1.0f));
    carWindow->createVertexArray();
    carWindow->translateShape(vec3(170, 150, 0));
    carWindow->scaleShape(vec3(40, 20, 1));

    ComplexShape2D* wheel = new Shape2D(50);
    wheel->setColor(color::BLACK);
    wheel->setMidColor(color::BLACK);
    Helper::buildCircle(0, 0, 1, 1, wheel);
    wheel->createVertexArray();
    wheel->translateShape(vec3(204, 143, 0));
    wheel->scaleShape(vec3(10, 10, 14));
    wheel->rotateShape(vec3(0, 1, 1), -15);

    Curve* human = new Curve();
    human->setColor(vec4(0.94, 0.76, 0.49, 1));
    human->readDataFromFile("resources/hermite/human.txt");
    human->buildHermite(vec4(0.94, 0.76, 0.49, 1), vec4(0.94, 0.76, 0.49, 1));
    human->createVertexArray();
    human->translateShape(vec3(170, 150, 0));
    human->scaleShape(vec3(32, 42, 1));

    player.car->addShape(carBody);
    player.car->addShape(carWindow);
    player.car->addShape(wheel);
    player.car->addShape(human);
    player.car->addShape(rwheel);
    player.car->addShape(fwheel);

    goal = new Curve();
    goal->readDataFromFile("./resources/hermite/star.txt");
    goal->buildHermite(color::YELLOW, color::YELLOW);
    goal->createVertexArray();
    auto pos = Helper::getRandomPosition2D(pair<int, int>(1500, 1500), pair<int, int>(80, ROADLIMIT - 100));
    goal->translateShape(vec3(pos, 0));
    goal->scaleShape(vec3(35, 35, 1));
    goal->setSolid();

    // Creates the drawing scenes with the projection matrix
    shapeScene.addShape2dToScene(road, roadShader);
    shapeScene.addShape2dToScene(goal, shader);
    shapeScene.addShape2dToScene(player.car, shader);

    for (int i = 0; i < gameLevel; i++)
    {
        auto pos = Helper::getRandomPosition2D(pair<int, int>(WIDTH, WIDTH), pair<int, int>(80, 500));
        buildBullet(pos);
    }

    // Initialize texts in the window
    Text textLevel = Text(projection, "Level: ", 60);
    textLevel.setPosition(vec2(40, 820));
    textLevel.setColor(color::WHITE);
    textLevel.initializeTextRender();
    textLevel.createVertexArray();

    textLevel.appendText(to_string(gameLevel));

    Text textAmmo = Text(projection, textAmmoPrefix, 60);
    textAmmo.setPosition(vec2(1300, 820));
    textAmmo.setColor(color::WHITE);
    textAmmo.initializeTextRender();
    textAmmo.createVertexArray();

    textAmmo.appendText(to_string(player.ammo));

    Shader textShader = Shader("./resources/textVertexShader.glsl", "./resources/textFragmentShader.glsl");

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
        if (player.ammo > 0)
        {
            player.ammo--;

            // shoot projectile
            Curve* proj = new Curve();
            proj->readDataFromFile("./resources/hermite/boomerang.txt");
            proj->buildHermite(color::BLUE, color::BLUE);
            proj->createVertexArray();

            auto pos = player.car->getBoundingBox();
            proj->translateShape(vec3(pos.second.x + 50, (pos.second.y - pos.first.y) / 2 + pos.first.y, 0));
            proj->scaleShape(vec3(150, 150, 1));
            proj->setSolid();

            Shader shader("./resources/vertexShader.glsl", "./resources/fragmentShader.glsl");
            shapeScene.addShape2dToScene(proj, shader, ShapeType::PROJ);

            // text rendering
            string result = textAmmoPrefix;
            result.append(to_string(player.ammo));
            textScene.getSceneElements()[1].first.setText(result);
        }
    }
}

void Game::processGameInput(float deltaTime, Window window)
{
    if (this->state == GAME_ACTIVE)
    {
        float playerVelocity = 7 * deltaTime;
        auto points = player.car->getBoundingBox();
        auto botLeft = points.first;
        auto topRight = points.second;

        if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && topRight.y < ROADLIMIT - 50)
        {
            player.car->transformShapes(vec3(0, 1, 0),
                    vec3(1), vec3(0), playerVelocity);
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS && botLeft.y > 0 + 50 + 25)
        {
            player.car->transformShapes(vec3(0, -1, 0),
                    vec3(1), vec3(0), playerVelocity); 
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS && botLeft.x > 0)
        {
            player.car->transformShapes(vec3(-1, 0, 0),
                    vec3(1), vec3(0), playerVelocity); 
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS && topRight.x < this->width)
        {
            player.car->transformShapes(vec3(1, 0, 0),
                    vec3(1), vec3(0), playerVelocity);  
        }
        glfwSetKeyCallback(window.getWindow(), playerShoot);
    }
    else 
    {
        glfwSetKeyCallback(window.getWindow(), NULL);
    }
}

void waitForUserInput(Game* game)
{
    Text textGameOver = Text(projection, "Game Over!", 60);
    textGameOver.setPosition(vec2(630, 720));
    textGameOver.initializeTextRender();
    textGameOver.createVertexArray();

    Shader shader = Shader("resources/textVertexShader.glsl", "resources/textFragmentShader.glsl");
    textScene.addTextToScene(textGameOver, shader);
    game->state = GameState::GAME_PAUSE;
}

bool Game::isOutOfBounds(vec2 pos)
{
    return pos.x < 0 || pos.x > WIDTH || pos.y < 0 || pos.y > HEIGHT;
}

static float rotVal = 0;

void Game::updateGame(float deltaTime)
{
    roadShader.use();
    glUniform1f(timeLoc, glfwGetTime());
    int index = 0;
    auto count = 0;
    for (auto elem: shapeScene.getSceneElements())
    {
        if (elem.type == ShapeType::BULLET)
        {
            if (this->isOutOfBounds(((Bullet*)elem.shape)->getTopCorner()) && 
                    this->isOutOfBounds(((Bullet*)elem.shape)->getBotCorner()))
            {
                elem.shape->clearShape();
                elem.shape->setDestroyed();
            }
            for (auto shape: player.car->getShapes())
            {
                if (((Bullet*)elem.shape)->checkShapesCollision(shape))
                {
                    player.car->setDestroyed();
                    this->state = GAME_PAUSE;
                    waitForUserInput(this);
                }

            }
            elem.shape->runAction();
        }

        if (elem.type == ShapeType::PROJ)
        {
            for (auto plpro: shapeScene.getSceneElements())
            {
                if (plpro.type == ShapeType::BULLET)
                {
                    // checks collisions with the player projectile
                    if (((Bullet*)plpro.shape)->checkShapesCollision(elem.shape))
                    {
                        plpro.shape->setDestroyed();
                        elem.shape->setDestroyed();
                    }
                }
            }
            // rotate boomerang
            auto pos = elem.shape->getPosition();
            pos.x += 400 * deltaTime;
            elem.shape->setModelMatrix(mat4(1));
            elem.shape->translateShape(pos);
            elem.shape->scaleShape(vec3(150));
            elem.shape->rotateShape(vec3(0, 0, 1), elem.shape->rotVal += 10);
        }

        if (!elem.shape->isAlive())
        {
            shapeScene.removeElement(index - count++);
        }

        index++;
    }

    for (auto shape: player.car->getShapes())
    {
        // checks if player reach the goal
        if (shape->checkCollision(goal))
        {
            player.ammo++;
            this->clearGame();
            this->initGame();
            this->renderGame();
        }
    }
}

void Game::renderGame()
{
    shapeScene.drawScene();
    textScene.drawScene();
}

void Game::clearGame()
{
    shapeScene.clear();
    textScene.clear();
}

ComplexShape2D* playButton;
ComplexShape2D* quitButton;

void Game::initMenu()
{
    wWidth = this->width;
    wHeight = this->height;
    ComplexShape2D* road = new Square(color::WHITE);
    road->scaleShape(vec3(this->width, this->height, 1));
    road->createVertexArray(); 

    // Create the background shader and set the window resolution for drawing purpose
    roadShader = Shader("resources/vertexShader.glsl", "resources/backFragShader.glsl");
    roadShader.use();
    GLuint resLoc = glGetUniformLocation(roadShader.getId(), "resolution");
    glUniform2f(resLoc, this->width, ROADLIMIT);
    timeLoc = glGetUniformLocation(roadShader.getId(), "iTime");
    glUniform1f(timeLoc, glfwGetTime());

    Text textGameName = Text(projection, GAME_NAME, 80);
    textGameName.setPosition(vec2(720, 800));
    textGameName.initializeTextRender();
    textGameName.createVertexArray();

    Text textPlay = Text(projection, "Play", 60);
    textPlay.setPosition(vec2(730, 260));
    textPlay.initializeTextRender();
    textPlay.createVertexArray();

    Text textQuit = Text(projection, "Quit", 60);
    textQuit.setPosition(vec2(730, 160));
    textQuit.initializeTextRender();
    textQuit.createVertexArray();

    Shader textShader = Shader("./resources/textVertexShader.glsl", "./resources/textFragmentShader.glsl");

    textScene.addTextToScene(textGameName, textShader);
    textScene.addTextToScene(textPlay, textShader);
    textScene.addTextToScene(textQuit, textShader);

    // buttons for the clickable texts
    playButton = new Square(color::NONE);
    playButton->createVertexArray();
    auto playWidth = textPlay.getTotalWidth();
    auto playHeight = textPlay.getTotalHeight();
    playButton->translateShape(vec3(740 + playWidth / 2, 260 + playHeight / 2, 0));
    playButton->scaleShape(vec3(playWidth / 2, playHeight / 2, 0));

    quitButton = new Square(color::NONE);
    quitButton->createVertexArray();
    auto quitWidth = textQuit.getTotalWidth();
    auto quitHeight = textQuit.getTotalHeight();

    quitButton->translateShape(vec3(740 + quitWidth / 2, 160 + quitHeight / 2, 0));
    quitButton->scaleShape(vec3(quitWidth / 2, quitHeight / 2, 0));

    Shader shader("resources/vertexShader.glsl", "resources/fragmentShader.glsl");

    shapeScene.addShape2dToScene(road, roadShader);
    shapeScene.addShape2dToScene(playButton, shader);
    shapeScene.addShape2dToScene(quitButton, shader);

    this->state = GAME_MENU;
}

void Game::updateMenu(float deltaTime)
{
    roadShader.use();
    glUniform1f(timeLoc, glfwGetTime());
}

void Game::renderMenu()
{
    shapeScene.drawScene();
    textScene.drawScene();
}

bool isMouseInButton(ComplexShape2D* button, int x, int y)
{
    bool collisionX, collisionY;
    collisionX = x >= button->getBotCorner().x && x <= button->getTopCorner().x;
    collisionY = y >= button->getBotCorner().y && y <= button->getTopCorner().y;
    return collisionX && collisionY;
}

bool playGame = false;
bool quitGame = false;

void mouseClick(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // adapting mouse coordinates to viewport coordinates
        y = wHeight - y;

        // mouse clicked play button
        playGame = isMouseInButton(playButton, x, y);
        quitGame = isMouseInButton(quitButton, x, y);
    }
}

void Game::processMouseInput(float deltaTime, Window window)
{
    glfwSetMouseButtonCallback(window.getWindow(), mouseClick);
    this->state = playGame ? GAME_ACTIVE : this->state;
    this->state = quitGame ? GAME_END : this->state;
}

void Game::clearMenu()
{
    shapeScene.clear();
    textScene.clear();
}
