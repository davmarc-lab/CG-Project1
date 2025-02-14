#pragma once

#include "Enemy.hpp"
#include "Entity.hpp"
#include <cassert>
#include <vector>

class MultiShape : public Enemy {
  public:
    inline unsigned int getAnchor() const { return this->m_anchor; }
    inline void setAnchor(const unsigned int &index) {
        assert(index < this->m_entities.size());
        this->m_anchor = index;
    }

    inline std::vector<Shared<Entity>> getEntities() const { return this->m_entities; }
    inline unsigned int addEntity(const Shared<Entity> &ett) {
        this->m_entities.push_back(ett);
        return this->m_entities.size() - 1;
    }

    inline Shared<Entity> getEntityByIndex(const unsigned int &index) const {
        assert(index < this->m_entities.size());
        return this->m_entities.at(index);
    }

    void moveEntities(const glm::vec3 &pos);

    virtual void refreshBounds() override;

    MultiShape() = delete;

    MultiShape(const ogl::ShaderProgram &shader, const Shared<Entity> &anchor);

    ~MultiShape() = default;

  private:
    std::vector<Shared<Entity>> m_entities{};
    unsigned int m_anchor = 0;
};
