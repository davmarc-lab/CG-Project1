#pragma once

#include "Layer.hpp"
#include "Utils.hpp"
#include "Window.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ogl {
	const ImGuiConfigFlags DEFAULT_IMGUI_CONFIGS{ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable};

    /**
     * @brief ImGui wrap to create a simple ImGui panel.
     * Objects extending this class should be used in the ImGuiManger
     *
     * @see ImGuiManager
     */
	class ImGuiPanel {
	public:
        /**
         * @brief Retrieves layer priority.
         *
         * The priority is used to organize layer callback methods.
         *
         * @return the layer priority
         */
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

		/**
		 * @brief Method called at every frame that draws the ImGui window.
		 *
		 * When extending this class this method can be ovverrided. When an
		 * object of this class is instanced, the method setRenderFunc() must be called.
		 */
		inline virtual void onRender() { this->m_renderFunc(); }

		ImGuiPanel(const std::string &name = "Panel", const unsigned short &priority = 0) :
			m_name(std::move(name)), m_priority(priority) {}

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

		ImGuiManager(const std::string &layerName, Window *window, const ImGuiConfigFlags &flags = 0) :
			Layer(std::move(layerName)), m_window(window), m_flags(flags) {}

		ImGuiManager(Window *window, const ImGuiConfigFlags &flags = DEFAULT_IMGUI_CONFIGS) :
			Layer("ImGui Manager"), m_flags(flags), m_window(window) {}

		template <typename T, typename... Args>
		inline Shared<T> addPanel(Args &&...args) {
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

		template <typename T>
		inline bool removePanel(const Shared<ImGuiPanel> &panel) {
			auto prio = panel->getPriority();
			auto elem = std::find(ALL(this->m_panels.at(prio)), panel);
			if (elem != this->m_panels.at(prio).end()) {
				this->m_panels.at(prio).erase(elem);
				return true;
			}
			return false;
		}

		inline void addPanel(const Shared<ImGuiPanel> &panel) {
			if (this->m_panels.find(panel->getPriority()) == this->m_panels.end()) {
				this->m_panels.insert_or_assign(panel->getPriority(), std::vector<Shared<ImGuiPanel>>());
			}

			this->m_panels.at(panel->getPriority()).push_back(panel);
		}

		std::vector<Shared<ImGuiPanel>> getPanels(const unsigned short &priority) { return {this->m_panels.at(priority)}; }

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate() override;
		virtual void onRender() override;

		virtual void begin() override;
		virtual void end() override;

	private:
		ImGuiConfigFlags m_flags;
		Window *m_window;

		std::map<unsigned short, std::vector<Shared<ImGuiPanel>>> m_panels{};
	};

} // namespace ogl
