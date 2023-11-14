#include "Lib.hpp"
#include "Shape/ComplexShape2D.hpp"
#include "Shape/Curve.hpp"
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

void initWindowView(Shader shader)
{
    mat4 view = translate(view, vec3(0.0f, 0.0f, -3));
    mat4 projection = ortho(0.0f, (float)WIDTH, 0.0f, float(HEIGHT));

    GLuint projLoc = glGetUniformLocation(shader.getId(), "projection");

    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
}

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

mat4 projection = ortho(0.0f, (float)WIDTH, 0.0f, float(HEIGHT));

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
    if (w.initializeWindow() != 0)
    {
        cout << "Cannot start the application, due to GLFW error" << endl;
        return -1;
    }
    else
    {   
        // Create the background shader and set the window resolution for drawing purpose
        Shader roadShader("resources/vertexShader.vert", "resources/backFragShader.frag");
        roadShader.use();
        GLuint resLoc = glGetUniformLocation(roadShader.getId(), "resolution");
        glUniform2f(resLoc, WIDTH, HEIGHT);

        // Creates the shapes shader
        Shader shader("resources/vertexShader.vert", "resources/fragmentShader.frag");

        // Create all the shapes of the scene
        ComplexShape2D* c = new Square();
        c->scaleShape(vec3(1600, 900 / 2, 1));
        c->createVertexArray();

        ComplexShape2D* player = new Shape2D(50);
        player->setColor(color::WHITE);
        player->setMidColor(color::RED);
        buildCircle(0, 0, 1.0, 1.0, player);
        player->createVertexArray();

        player->translateShape(vec3(200, 200, 0));
        player->scaleShape(vec3(25, 25, 1));

        ComplexShape2D* enemy = new Shape2D(50);
        enemy->setColor(color::BLACK);
        enemy->setMidColor(color::WHITE);
        buildCircle(0, 0, 1.0, 1.0, enemy);

        enemy->createVertexArray();
        enemy->translateShape(vec3(1400, 200, 0));
        enemy->scaleShape(vec3(25, 25, 1));

        Curve* herm = new Curve();
        herm->readDataFromFile("resources/hermite/boomerang.txt");
        herm->buildHermite(color::BLACK, color::RED, herm);

        herm->createVertexArray();

        herm->translateShape(vec3(500, 200, 0));
        herm->scaleShape(vec3(200, 200, 1));

        cout << to_string(herm->getModelMatrix()) << endl;

        // Creates the drawing scenes with the projection matrix
        Scene scene = Scene(projection);
        scene.addShape2dToScene(c, roadShader);
        scene.addShape2dToScene(player, shader);
        scene.addShape2dToScene(herm, shader);

        // Start of window loop
        while (!glfwWindowShouldClose(w.getWindow()))
        {
            // input
            w.processCloseInput();
            processPlayerInput(w, player);

            // render
            glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            scene.drawScene();

            // swap buffers and poll IO events
            glfwSwapBuffers(w.getWindow());
            glfwPollEvents();
        }
        w.terminateWindow();
    }

    return 0;
}
