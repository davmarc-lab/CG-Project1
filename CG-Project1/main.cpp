#include "Game/Game.hpp"
#include "Lib.hpp"
#include "Shape/ComplexShape2D.hpp"
#include "Window/Window.hpp"

#include <iostream>

#define PI 3.14159265358979323846

const int WIDTH = 1600, HEIGHT = 900;

void buildCircle(float cx, float cy, float raggiox, float raggioy, ComplexShape2D* fig)
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

bool checkOutOfBounds(vec3 pos)
{
    cout << pos.x << ", " << pos.y << endl;
    if (pos.x > WIDTH || pos.x < 0 || pos.y > 500 || pos.y < 0)
        return true;
    return false;
}

// keep the player in the box
void processPlayerInput(Window window, ComplexShape2D* player)
{
    float velocity = 0.2;
    int pixel = 1;

    float width = window.getResolution().x;
    float height = window.getResolution().y;

    vec3 pos = vec3(player->getModelMatrix()[3]);

    if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS && pos.y < height / 2 - 50 - 25)
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
}


void rotateObject(ComplexShape2D* shape)
{
    float velocity = 3.5f;
    shape->rotateShape(vec3(0, 0, 1), velocity);
}

int main()
{

    Game game = Game(WIDTH, HEIGHT);
    Window window = Window("Mario Kart", WIDTH, HEIGHT);

    // Initialize all game object and window
    game.init();

    // Start of window loop
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        // input
        window.processCloseInput();
        /* processPlayerInput(window, player); */

        // render
        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.render();

        /* enemHelper.enemyMoveAction(enemy); */
        /* // bananaHelper.enemyMoveAction(herm); */
        /* rotateObject(herm); */

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    window.terminateWindow();
    return 0;
}
