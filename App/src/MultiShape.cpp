#include "../include/MultiShape.hpp"

MultiShape::MultiShape(const ogl::ShaderProgram &shader, const Shared<Entity> &anchor) {
    this->setPosition(anchor->getPosition());
    this->setScale(anchor->getScale());
    this->setRotation(anchor->getRotation());
    this->m_entities.push_back(anchor);

    this->setRenderCall([&shader, this]() {
        for (auto e : this->m_entities) {
            e->render();
        }
    });
}

void MultiShape::moveEntities(const glm::vec3 &pos) {
    auto origin = this->m_entities.at(this->m_anchor)->getPosition();
    this->setPosition(pos);
    this->m_entities.at(this->m_anchor)->setPosition(pos);
    for (auto i = 0; i < this->m_entities.size(); i++) {
        if (i != this->m_anchor) {
            auto pre = this->m_entities[i]->getPosition();
            this->m_entities[i]->setPosition(pos + (pre - origin));
        }
    }
}

void MultiShape::refreshBounds() {
    auto bot = glm::vec3(1, 1, 0);
    bool first = true;

    for (auto e : this->m_entities) {
        for (auto v : e->getCoordsVector()) {
            auto model = e->getModelMatrix() * glm::vec4(v, 1);
            if (first) {
                bot = model;
                first = false;
            }
            bot.x = bot.x >= model.x ? model.x : bot.x;
            bot.y = bot.y >= model.y ? model.y : bot.y;
        }
    }

    auto top = glm::vec3(1, 1, 0);
    first = true;
    for (auto e : this->m_entities) {
        for (auto v : e->getCoordsVector()) {
            auto model = e->getModelMatrix() * glm::vec4(v, 1);
            if (first) {
                top = model;
                first = false;
            }
            top.x = top.x <= model.x ? model.x : top.x;
            top.y = top.y <= model.y ? model.y : top.y;
        }
    }

    this->m_bounds.botLeft = bot;
    this->m_bounds.topRight = top;
}
