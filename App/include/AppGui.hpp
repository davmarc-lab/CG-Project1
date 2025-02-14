#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "Scene.hpp"

class ImGuiStats : public ogl::ImGuiPanel {
  public:
    ImGuiStats() : ogl::ImGuiPanel("Stats", 0) {}

    virtual ~ImGuiStats() = default;

  private:
};

class ImGuiScene : public ogl::ImGuiPanel {
  public:
    ImGuiScene(Scene &scene) : ogl::ImGuiPanel("Scene", 0), m_scene(scene) {}

    virtual ~ImGuiScene() = default;

  private:
    Scene &m_scene;
};
