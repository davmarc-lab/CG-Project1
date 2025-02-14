#pragma once

#include <string>

namespace ogl {
class Layer {
  public:
    Layer(const std::string &name) : m_name(std::move(name)) {}
    Layer() : Layer("Simple Layer") {}
    Layer(const Layer &other) = delete;
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}

    virtual void onUpdate() {}
    virtual void onRender() {}

    virtual void begin() {}
    virtual void end() {}

    std::string getName() const { return this->m_name; }

    bool isAttached() const { return this->m_attached; }

  protected:
    std::string m_name{"Layer"};
    bool m_attached = false;
};
} // namespace ogl
