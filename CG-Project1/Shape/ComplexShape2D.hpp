#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Shader/Shader.hpp"

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
        
        float rotVal = 0;

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

        // Retrieves the top right corner of the shape.
        vec2 getTopCorner()
        {
            vec4 xmax = vec4(-1), ymax = vec4(-1);
            vec2 point = vec2(0);
            auto points = this->getVertexArray();

            // this method transform the normalized points in world space coordinates and find the max.
            for (int i = 0; i < points.size(); i++)
            {
                auto var = this->getModelMatrix() * vec4(points[i], 1);
                xmax = xmax.x <= var.x ? var : xmax;
            }
            for (int i = 0; i < points.size(); i++)
            {
                auto var = this->getModelMatrix() * vec4(points[i], 1);
                ymax = ymax.y <= var.y ? var : ymax;
            } 

            point = vec2(xmax.x, ymax.y);

            return point;
        }

        // Retrieves the bottom left corner of the shape.
        vec2 getBotCorner()
        {
            vec4 xmin = vec4(-1), ymin = vec4(-1);
            vec2 point = vec2(0);

            auto points = this->getVertexArray();

            // this method transform the normalized points in world space coordinates and find the min.
            for (int i = 0; i < points.size(); i++)
            {
                auto var = this->getModelMatrix() * vec4(points[i], 1);
                if (xmin.x == -1)
                    xmin = var;
                else
                    xmin = xmin.x >= var.x ? var : xmin;
            }
            for (int i = 0; i < points.size(); i++)
            {
                auto var = this->getModelMatrix() * vec4(points[i], 1);
                if (ymin.y == -1)
                    ymin = var;
                else
                    ymin = ymin.y >= var.y ? var : ymin;
            } 

            point = vec2(xmin.x, ymin.y);

            return point;
        }

        // Sets the shape as solid, so needs to be checked on collision system
        void setSolid() { this->isSolid = true; }

        /*
         * Collision system based on AABB collisions.
         * Retrieves if this shape is in collision with another one given.
         */
        bool checkCollision(ComplexShape2D* shape)
        {
            if (shape->isSolid && shape->isAlive() && this->isAlive())
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

        // Destroy this shape.
        void setDestroyed() 
        {
            this->isDestroyed = true;
            this->clearShape();
        }

        // Sets this shape to alive state.
        void setAlive() { this->isDestroyed = false; }

        // Retrieves if the shape is alive.
        bool isAlive() { return !this->isDestroyed; }

        // Retrieves if the shape is collidable.
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

        // Return the world coordinates of the shape.
        vec3 getPosition() { return this->model[3]; }

        // This method clear the vertex vector.
        void clearVertexArray() { this->vertex.clear(); }

        // This method clear the color vector.
        void clearColorArray() { this->colors.clear(); }

        // Clears the shape entity.
        virtual void clearShape() = 0;

        // Creates the VAO of the shape
        virtual void createVertexArray() = 0;

        // Create all vertex and color VBO, enable them and draw in the windows
        virtual void draw(Shader shader) = 0;

        // Defines a constant action done by the shape.
        virtual void runAction() = 0;
};
