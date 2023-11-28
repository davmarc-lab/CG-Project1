#pragma once

#include "../Lib.hpp"

#include "../Text/SimpleText.hpp"
#include <vector>

class TextScene
{
    private:
        vector<pair<Text, Shader>> scene;
        mat4 projection;

    public:
        TextScene(mat4 projection);

        TextScene();

        void addTextToScene(Text text, Shader shader) { this->scene.push_back(pair<Text, Shader>(text, shader)); }

        vector<pair<Text, Shader>> getSceneElements() { return this->scene; }

        void drawScene();

        void clear();
};
