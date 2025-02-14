#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

struct EntityVertex {
    EntityVertex() = default;

    EntityVertex(const std::vector<glm::vec3> &coords, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices)
        : coords(coords), colors(colors), indices(indices) {}

    std::vector<glm::vec3> coords{};
    std::vector<glm::vec4> colors{};
    std::vector<unsigned int> indices{};
};

struct EntityBuffer {
    ogl::VertexArray vao{};
    ogl::VertexBuffer vbo_g{};
    ogl::VertexBuffer vbo_c{};
    ogl::ElementBuffer ebo{};
};

struct BoundingBox {
    glm::vec3 topRight{};
    glm::vec3 botLeft{};
};

class EntityModel {
  public:
    EntityModel() = default;

    EntityModel(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
        : position(position), scale(scale), rotation(rotation) {}

    void updateModelMatrix();

    glm::vec3 position{};
    glm::vec3 scale{1, 1, 1};
    glm::vec3 rotation{};
    glm::quat quat{};

    glm::mat4 model{1};
    bool dirty = true;
};

class Entity {
  public:
    void init();

    inline glm::vec3 getPosition() const { return this->m_model.position; }

    inline void setPosition(const glm::vec3 &pos) {
        this->m_model.position = pos;
        this->m_model.dirty = true;
    }

    inline glm::vec3 getScale() const { return this->m_model.scale; }

    inline void setScale(const glm::vec3 &scale) {
        this->m_model.scale = scale;
        this->m_model.dirty = true;
    }

    inline glm::vec3 getRotation() const { return this->m_model.rotation; }

    inline void setRotation(const glm::vec3 &rot) {
        this->m_model.rotation = rot;
        this->m_model.quat = glm::quat(glm::radians(rot));
        this->m_model.dirty = true;
    }

    inline glm::mat4 getModelMatrix() {
        if (this->m_model.dirty) {
            this->m_model.updateModelMatrix();
            this->m_model.dirty = false;
            if (this->m_collidable)
                this->refreshBounds();
        }
        return this->m_model.model;
    }

    inline std::vector<glm::vec3> getCoordsVector() const { return this->m_vertex.coords; }
    inline void setCoordsVector(const std::vector<glm::vec3> &coords) { this->m_vertex.coords = coords; }

    inline std::vector<glm::vec4> getColorVector() const { return this->m_vertex.colors; }
    inline void setColorVector(const std::vector<glm::vec4> &colors) { this->m_vertex.colors = colors; }

    inline std::vector<unsigned int> getIndicesVector() const { return this->m_vertex.indices; }
    inline void setIndicesVector(const std::vector<unsigned int> &indices) { this->m_vertex.indices = indices; }

    inline bool isCollidable() const { return this->m_collidable; }
    inline void setCollidable(const bool &collidable) {
        if (collidable) {
            // refresh bounding box
            this->refreshBounds();
        }
        this->m_collidable = collidable;
    }

    virtual void refreshBounds() {
        auto bot = glm::vec3(1, 1, 0);
        bool first = true;

        for (auto i = 0; i < this->m_vertex.coords.size(); i++) {
            auto elem = this->getModelMatrix() * glm::vec4(this->m_vertex.coords[i], 1);
            if (first) {
                bot = elem;
                first = false;
            }
            bot.x = bot.x >= elem.x ? elem.x : bot.x;
            bot.y = bot.y >= elem.y ? elem.y : bot.y;
        }

        auto top = glm::vec3(1, 1, 0);
        first = true;
        for (auto i = 0; i < this->m_vertex.coords.size(); i++) {
            auto elem = this->getModelMatrix() * glm::vec4(this->m_vertex.coords[i], 1);
            if (first) {
                top = elem;
                first = false;
            }
            top.x = top.x <= elem.x ? elem.x : top.x;
            top.y = top.y <= elem.y ? elem.y : top.y;
        }

        this->m_bounds.botLeft = bot;
        this->m_bounds.topRight = top;
    }

    virtual BoundingBox getBoundingBox() { return this->m_bounds; }

    inline void bindVAO() { this->m_buffers.vao.bind(); }

    inline void setRenderCall(std::function<void()> &&func) { this->m_renderCall = std::move(func); }

    virtual void render() {
        if (this->m_renderCall != nullptr)
            this->m_renderCall();
    }

    Entity() = default;

    Entity(const EntityVertex &vertex, const EntityModel &model) : m_vertex(vertex), m_model(model) {}

    ~Entity() = default;

  protected:
    EntityBuffer m_buffers{};
    EntityVertex m_vertex{};
    EntityModel m_model{};

    std::function<void()> m_renderCall = nullptr;

    bool m_init = false;
    bool m_collidable = false;
    BoundingBox m_bounds{};
};
