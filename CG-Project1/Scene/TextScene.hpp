#pragma once

#include "../Lib.hpp"

#include "../Text/SimpleText.hpp"
#include <vector>

// This class instance the text scene of the games.
class TextScene
{
    private:
        // all texts with their shader.
        vector<pair<Text, Shader>> scene;
        mat4 projection;

    public:
        // Constructs a scene with a given projection.
        TextScene(mat4 projection);

        TextScene() {}

        // Adds a Text object to the scene with his shader.
        void addTextToScene(Text text, Shader shader) { this->scene.push_back(pair<Text, Shader>(text, shader)); }

        // Retrieves all the scene elements.
        vector<pair<Text, Shader>>& getSceneElements() { return this->scene; }

        // Draws all the texts.
        void drawScene();

        void clear();
};
