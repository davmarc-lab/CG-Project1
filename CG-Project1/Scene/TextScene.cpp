#include "TextScene.hpp"

TextScene::TextScene(mat4 projection)
{
    this->projection = projection;
}

void TextScene::drawScene()
{
    for (auto elem: this->scene)
    {
        auto text = elem.first;
        auto shader = elem.second;

        shader.use();
        text.renderText(shader, elem.first.getPosition().x, elem.first.getPosition().y, 1);
    }
}

void TextScene::clear()
{
    this->scene.clear();
}
