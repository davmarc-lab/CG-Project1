#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Shader/Shader.hpp"
#include "../Collision/Collision.hpp"

/*
 * This abstract class provides a standard base for the most of basic shapes using VAO, geometry and
 * color VBO, mid and other color, modellation matrix.
 */
class ComplexShape2D
{
    public:
        // Standard VAO buffer.
        GLuint vao;
        // VBO buffer used for geometry.
        GLuint vbo_g;
        // VBO buffer used for colors.
        GLuint vbo_c;
        // Vector of all vertices of the shape.
        vector<vec3> vertex;
        // Vector of all color for each vertex.
        vector<vec4> colors;
        // The primary color of the shape.
        Color color;
        // The center vertex color of the shape.
        Color midColor;
        // Modellation matrix.
        mat4 model = mat4(1.0f);
        // Number of vertex of the shape.
        int nvertex = 0;
        // Number of triangles of the shape.
        int ntriangle = 0;

        // Says if the shape is solid or not
        bool isSolid = false;

        // Says if the shape is destroyed or alive
        bool isDestroyed = false;

        // Add element to vertex vector3.
        void addElementVertex(vec3 elem) { this->vertex.push_back(elem); }

        // Add element to colors vetcor4.
        void addElementColors(vec4 elem) { this->colors.push_back(elem); }

        // Retrieves the vertex number of the shape.
        int getVertexNum() { return this->nvertex; }

        // This method set the number of vertices.
        void setVertexNum(int n) { this->nvertex = n; }

        // Retrieves the triangles nummber of the shape.
        int getTriangleNum() { return this->ntriangle; }

        // This method sets the number of triangles of the shape.
        void setTriangleNum(int n) { this->ntriangle = n; }

        // This method sets the primary color of the shape.
        void setColor(Color color) { this->color = color; }

        // Retrieves the primary color of the shape.
        Color getColor() { return this->color; }

        // This method sets the center vertex color of the shape.
        void setMidColor(Color mid) { this->midColor = mid; }

        // Retrieves the center vertex color of the shape.
        Color getMidColor() { return this->midColor; }

        // Retrieves the vector of vertices of the shape.
        vector<vec3> getVertexArray() { return this->vertex; }

        // This method sets the vector of vertices of the shape.
        void setVertexArray(vector<vec3> vertex) { this->vertex = vertex; }

        // This method sets the vector of colors of the shape.
        void setColorsArray(vector<vec4> colors) { this->colors = colors; }

        // Retrieves the modellation matrix of the shape.
        mat4 getModelMatrix() { return this->model; }

        // This metod sets the model matrix.
        void setModelMatrix(mat4 model) { this->model = mat4(model); }

        void setSolid() { this->isSolid = true; }

        bool checkCollision(ComplexShape2D* shape)
        {
            if (shape->isSolid)
            {
                bool collisionX, collisionY;
                auto top = this->model * vec4(1, 1, 0, 1);
                auto firstTopPos = ivec2(top.x, top.y);
                auto bot = this->model * vec4(-1, -1, 0, 1);
                auto firstBotPos = ivec2(bot.x, bot.y);

                top = shape->getModelMatrix() * vec4(1, 1, 0, 1);
                auto secondTopPos = ivec2(top.x, top.y);
                bot = shape->getModelMatrix() * vec4(-1, -1, 0, 1);
                auto secondBotPos = ivec2(bot.x, bot.y);

                collisionX = firstBotPos.x <= secondTopPos.x && firstTopPos.x >= secondBotPos.x;
                collisionY = firstBotPos.y <= secondTopPos.y && firstTopPos.y >= secondBotPos.y;

                return collisionX && collisionY;
            }
            return false;
        }

        void setDestroyed() { this->isDestroyed = true; }

        void setAlive() { this->isDestroyed = false; }

        bool isAlive() { return !this->isDestroyed; }

        bool isColiision() { return this->isSolid; }

        // This method transform the model matrix for scaling puropose.
        void scaleShape(vec3 mod) { this->setModelMatrix(scale(this->getModelMatrix(), mod)); }

        // This method transform the model matrix for translating purpose.
        void translateShape(vec3 mod) { this->setModelMatrix(translate(this->getModelMatrix(), mod)); }

        // This method transform the model matrix for rotating purpose.
        void rotateShape(vec3 mod, float w) { this->setModelMatrix(rotate(this->getModelMatrix(), radians(w), mod)); }

        // Retrieves the VertexArrayObject of the shape.
        GLuint getVertexArrayObject() { return this->vao; }

        // Retrieves the vector of colors for each vertex.
        vector<vec4> getColorsArray() { return this->colors; }

        vec3 getPosition() { return this->model[3]; }

        vec3 getSize() { return vec3(this->model[0][0], this->model[1][1], this->model[2][2]); }

        // This method clear the vertex vector.
        void clearVertexArray() { this->vertex.clear(); }

        // This method clear the color vector.
        void clearColorArray() { this->colors.clear(); }

        virtual void clearShape() = 0;

        // Creates the VAO of the shape
        virtual void createVertexArray() = 0;

        // Create all vertex and color VBO, enable them and draw in the windows
        virtual void draw(Shader shader) = 0;
};
