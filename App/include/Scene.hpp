#pragma once
#include <vector>

#include "../../Opengl-Core/include/Core.hpp"

#include "Entity.hpp"

using namespace ogl;

class Scene : public Layer {
  public:
    void addEntity(const unsigned int &shader, const Shared<Entity> &ett);

    void removeEntity(const unsigned int &shader, const Shared<Entity> &ett);
    void removeEntity(const Shared<Entity> &ett);

    inline std::vector<Shared<Entity>> getAllEntities() const {
        auto res = std::vector<Shared<Entity>>{};
        for (auto [_, etts] : this->m_entities) {
            res.insert(res.end(), ALL(etts));
        }
        return res;
    }

    void drawBoundingBox(const unsigned int &shader, const BoundingBox &box, const glm::vec4 &color);

    inline bool isDebugEnabled() const { return this->m_debug; }
    inline void setDebug(const bool &val) { this->m_debug = val; }

    virtual void onAttach() override;

    virtual void onUpdate() override;

    virtual void onRender() override;

    Scene() = default;

    virtual ~Scene() override = default;

  private:
    std::unordered_map<unsigned int, std::vector<Shared<Entity>>> m_entities{};

    VertexArray m_vao{};
    VertexBuffer m_vbog{};
    std::vector<glm::vec3> m_coords{};
    VertexBuffer m_vboc{};
    std::vector<glm::vec4> m_colors{};
    bool m_debug = false;
};
