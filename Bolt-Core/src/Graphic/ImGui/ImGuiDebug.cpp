#include "../../../include/Graphic/ImGui/ImGuiDebug.hpp"

#include "../../../../Bolt-imgui/include/imgui/imgui.h"

namespace bolt {
	void ImGuiDebug::onRender() {
		ImGui::Begin(this->m_title.c_str());
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Average frame per seconds: %f", io.Framerate);
		ImGui::End();
	}
} // namespace bolt
