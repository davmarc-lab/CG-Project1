#include "../../include/Graphic/ImGuiLayer.hpp"

#include "../../../Bolt-Vendor/include/glfw/include/GLFW/glfw3.h"

#include "../../../Bolt-imgui/include/imgui/imgui.h"
#include "../../../Bolt-imgui/include/imgui/imgui_impl_glfw.h"
#include "../../../Bolt-imgui/include/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include "../../include/Graphic/Window.hpp"

namespace bolt {
	void ImGuiLayer::onAttach() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Vieports

		//float fontSize = 17.f;
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/DroidSans.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/Cousine-Regular.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/Karla-Regular.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/ProggyClean.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/ProggyTiny.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../Bolt-imgui/include/imgui/misc/fonts/Roboto-Medium.ttf", fontSize, NULL, io.Fonts->GetGlyphRangesDefault());

		// Setup Dear ImGui style
		// ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		style.WindowRounding = 10.0f;
		style.FrameRounding = 4.f;
		style.GrabRounding = 4.f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;

		const auto window = static_cast<GLFWwindow*>(this->m_window->getCurrentWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
        this->m_attached = true;
	}

	void ImGuiLayer::onDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onEvent(const Event& e) {
		// BT_INFO_CORE("No Implementation of Event handling.");
	}

	void ImGuiLayer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end() {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(this->m_window->getWidth(), this->m_window->getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			const auto backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
} // namespace bolt
