#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
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
#include "../Scene/Scene.hpp"
#include "../Utils/utils.hpp"
#include "../Text/SimpleText.hpp"

struct Player
{
    ComplexShape2D* shape;
    unsigned int hp = 3;
    int ammo = 0;
} player;

ComplexShape2D* goal;
vector<ComplexShape2D*> enemies;

vector<Helper> helpers;
vector<Text> textScene;

string textAmmoFormat = "Ammo: ";

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
    goal = new Square(color::YELLOW);
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

    goal->createVertexArray();
    goal->translateShape(vec3(1400, 200, 0));
    goal->scaleShape(vec3(25, 25, 1));
    goal->setSolid();

    // Creates the drawing scenes with the projection matrix
    scene.addShape2dToScene(road, roadShader);
    scene.addShape2dToScene(goal, shader);
    scene.addShape2dToScene(player.shape, shader);

    for (int i = 0; i < gameLevel; i++)
    {
        ComplexShape2D* enem = new Shape2D(3);
        enem->setColor(color::WHITE);
        Helper::buildTriangle(enem);
        enem->createVertexArray();

        auto pos = Helper::getRandomPosition2D(pair<int, int>(WIDTH, WIDTH), pair<int, int>(80, 500));
        enem->setModelMatrix(mat4(1.0f));
        enem->translateShape(vec3(pos.x, pos.y, 0));
        enem->scaleShape(vec3(25, 25, 1));
        enem->rotateShape(vec3(0, 0, 1), 90);
        enem->setSolid(); 

        scene.addShape2dToScene(enem, shader, ShapeType::ENEMY);
        auto tmp = Helper(vec2(WIDTH, HEIGHT));
        // idk i need a random velocity generator
        tmp.setVelocity((pow(sin(glfwGetTime()), 2) * (rand() % 4)) + 6);
        helpers.push_back(tmp);
    }

    // Initialize texts in the window
    Text textLevel = Text(projection, "Level: ", 60);
    textLevel.setPosition(vec2(40, 820));
    textLevel.initializeTextRender();
    textLevel.createVertexArray();

    textLevel.appendText(to_string(gameLevel));

    Text textAmmo = Text(projection, textAmmoFormat, 60);
    textAmmo.setPosition(vec2(1300, 820));
    textAmmo.initializeTextRender();
    textAmmo.createVertexArray();

    textAmmo.appendText(to_string(player.ammo));

    textScene.push_back(textLevel);
    textScene.push_back(textAmmo);

    this->state = GAME_ACTIVE;
    gameLevel++;

    glDeleteShader(shader.getId());
    glDeleteShader(roadShader.getId());
    /* bananaHelper.setYVelocity(0.05f); */
}

void playerShoot(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        player.ammo--;
        string result = textAmmoFormat;
        result.append(to_string(player.ammo));
        textScene[1].setText(result);
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

    // updates all enemies position and informations
    int k = 0;
    for (int i = 0; i < scene.getSceneElements().size(); i++)
    {
        auto elem = scene.getSceneElements()[i];
        if (elem.type != ShapeType::ENEMY)
            continue;

        auto pos = elem.shape->getPosition();
        elem.shape->setModelMatrix(mat4(1.0f));
        elem.shape->setModelMatrix(translate(elem.shape->getModelMatrix(), pos - vec3(helpers[k].getVelocity(), 0, 0)));
        elem.shape->scaleShape(vec3(25, 25, 1));

        if (player.shape->checkCollision(elem.shape))
        {
            player.shape->setDestroyed();
        }

        elem.shape->rotateShape(vec3(0, 0, 1), 90);

        k++;
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
    Shader textShader = Shader("./resources/textVertexShader.vert", "./resources/textFragmentShader.frag");
    scene.drawScene();

    for (auto elem: textScene)
    {
        elem.renderText(textShader, elem.getPosition().x, elem.getPosition().y, 1, vec4(1, 0, 0, 1));
    }

}

void Game::clear()
{
    scene.clear();
    helpers.clear();
    textScene.clear();
}

