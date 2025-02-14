#pragma once

#include "Layer.hpp"
#include "Utils.hpp"
#include "Window.hpp"

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ogl {

class ImGuiPanel {
  public:
    inline unsigned short getPriority() const { return this->m_priority; }

    inline void setPriority(const unsigned short &priority) {
        // EVENT: ig_layer_priority_changed
        this->m_priority = priority;
    }

    inline virtual void onAttach() {
        if (this->m_attached)
            return;
        this->m_attached = true;
    }

    inline void setRenderFunc(std::function<void()> &&func) { this->m_renderFunc = std::move(func); }

    inline virtual void onRender() { this->m_renderFunc(); }

    ImGuiPanel(const std::string &name = "Panel", const unsigned short &priority = 0)
        : m_name(std::move(name)), m_priority(priority) {}

    virtual ~ImGuiPanel() = default;

  private:
    std::string m_name = "Panel";
    unsigned short m_priority = 0;
    bool m_attached = false;

    std::function<void()> m_renderFunc = []() {};
};

class ImGuiManager : public Layer {
  public:
    ImGuiManager() = delete;

    ImGuiManager(const std::string &layerName, Window &window, const ImGuiConfigFlags &flags = 0)
        : Layer(std::move(layerName)), m_window(window), m_flags(flags) {}

    ImGuiManager(Window &window, const ImGuiConfigFlags &flags)
        : Layer("ImGui Manager"), m_flags(flags), m_window(window) {}

    template <typename T, typename... Args> inline Shared<T> addPanel(Args &&...args) {
        if (!this->m_attached)
            return nullptr;

        Shared<T> elem = CreateShared<T>(std::forward<Args>(args)...);
        elem->onAttach();

        if (this->m_panels.find(elem->getPriority()) == this->m_panels.end()) {
            this->m_panels.insert_or_assign(elem->getPriority(), std::vector<Shared<ImGuiPanel>>());
        }

        this->m_panels.at(elem->getPriority()).push_back(elem);
        return elem;
    }

    std::vector<Shared<ImGuiPanel>> getPanels(const unsigned short &priority) { return {this->m_panels.at(priority)}; }

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate() override;
    virtual void onRender() override;

    virtual void begin() override;
    virtual void end() override;

  private:
    ImGuiConfigFlags m_flags =
        ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    Window &m_window;

    std::map<unsigned short, std::vector<Shared<ImGuiPanel>>> m_panels{};
};

} // namespace ogl
