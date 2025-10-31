#pragma once

#include "../vendor/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../vendor/include/imgui/imgui.h"
#include "../vendor/include/imgui/imgui_impl_glfw.h"
#include "../vendor/include/imgui/imgui_impl_opengl3.h"

/**
* @brief Create an help icon with the given descrition
*
* @param desc help description
*/
inline static void ImGuiHelpMarker(const char *desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
