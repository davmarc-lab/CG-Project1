#include "../include/Scene.hpp"
#include <algorithm>

void Scene::addEntity(const unsigned int &shader, const Shared<Entity> &ett) {
    if (this->m_entities.find(shader) == this->m_entities.end()) {
        this->m_entities.insert_or_assign(shader, std::vector<Shared<Entity>>{});
    }
    this->m_entities.at(shader).push_back(ett);
}

void Scene::removeEntity(const unsigned int &shader, const Shared<Entity> &ett) {
    auto elem = this->m_entities.find(shader);
    if (elem == this->m_entities.end()) {
        return;
    }
    elem->second.erase(std::find(ALL(elem->second), ett));
}

void Scene::removeEntity(const Shared<Entity> &ett) {
    for (auto [key, etts] : this->m_entities) {
        auto elem = std::find(ALL(etts), ett);
        if (elem != etts.end()) {
            this->removeEntity(key, ett);
            break;
        }
    }
}

void Scene::onAttach() {
    this->m_vao.onAttach();
    this->m_vbog.onAttach();
    this->m_vboc.onAttach();
}

void Scene::drawBoundingBox(const unsigned int &shader, const BoundingBox &box, const glm::vec4 &color) {
    this->m_vao.bind();

    this->m_coords.clear();
    this->m_coords.push_back(box.botLeft);
    this->m_coords.push_back({box.topRight.x, box.botLeft.y, 0});
    this->m_coords.push_back({box.topRight.x, box.botLeft.y, 0});
    this->m_coords.push_back(box.topRight);
    this->m_coords.push_back(box.topRight);
    this->m_coords.push_back({box.botLeft.x, box.topRight.y, 0});
    this->m_coords.push_back({box.botLeft.x, box.topRight.y, 0});
    this->m_coords.push_back(box.botLeft);
    for (int i = 0; i < 8; i++)
        this->m_colors.push_back(color);

    this->m_vbog.setup(this->m_coords.data(), this->m_coords.size(), GL_STATIC_DRAW);
    this->m_vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    this->m_vboc.setup(this->m_colors.data(), this->m_colors.size(), GL_STATIC_DRAW);
    this->m_vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &glm::mat4(1)[0][0]);
    glDrawArrays(GL_LINES, 0, this->m_coords.size());
}

void Scene::onUpdate() {}

void Scene::onRender() {
    for (auto [shader, etts] : this->m_entities) {
        glUseProgram(shader);
        for (auto e : etts) {
            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &e->getModelMatrix()[0][0]);
            e->render();
            if (this->m_debug) {
                if (e->isCollidable()) {
                    this->drawBoundingBox(shader, e->getBoundingBox(), {1, 0, 0, 1});
                }
            }
        }
    }
}
