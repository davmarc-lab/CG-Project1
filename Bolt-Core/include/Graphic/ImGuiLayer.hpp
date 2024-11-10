#pragma once

#include "../Core/Layer.hpp"

#include "../../../Bolt-imgui/include/imgui/imgui.h"
#include "../../../Bolt-imgui/include/imgui/imconfig.h"

#include "Window.hpp"

namespace bolt {
	class ImGuiLayer final : public Layer {
	private:
		Shared<Window> m_window;

	public:
		explicit ImGuiLayer(Shared<Window> w) : Layer("ImGui"), m_window(std::move(w)) {}

		virtual void onAttach() override;

		virtual void onDetach() override;

		virtual void onEvent(const Event& e) override;

		virtual void begin() override;

		virtual void end() override;
	};
}
