#pragma once

#include "../../Application/Application.hpp"

#include "../../Core/Layer.hpp"
#include "../../Core/Utils.hpp"

#include <stdexcept>
#include <string>

namespace bolt {
	class ImGuiDebug : public Layer {
	public:
		ImGuiDebug(std::string title = "Debug") : m_title(std::move(title)) {
			if (!Application::isImGuiEnabled()) // PUT LOG HERE
				throw std::runtime_error("ImGui is disabled.");
		}

		virtual ~ImGuiDebug() override = default;

		virtual void onRender() override;

		void setVisible(const b8& visible) { this->m_showing = visible; }

		b8 isOpened() const { return this->m_showing; }

	private:
		std::string m_title;
		b8 m_showing = false;
	};
} // namespace bolt
