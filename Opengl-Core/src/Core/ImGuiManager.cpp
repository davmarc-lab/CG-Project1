#include "../../include/Core/ImGui.hpp"

namespace ogl {

void ImGuiManager::onAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags = this->m_flags;

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 10.f;
    style.FrameRounding = 4.f;
    style.GrabRounding = 4.f;
    style.Colors[ImGuiCol_WindowBg].w = 0.8f;

    ImGui_ImplGlfw_InitForOpenGL(this->m_window.getContext(), true);
    ImGui_ImplOpenGL3_Init();

    this->m_attached = true;
}
void ImGuiManager::onDetach() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::onUpdate() {}

void ImGuiManager::onRender() {
    for (auto [_, second] : this->m_panels) {
        for (auto panel : second) {
            panel->onRender();
        }
    }
}

void ImGuiManager::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiManager::end() {
    ImGuiIO &io = ImGui::GetIO();
    // use event listener when window size is changed
    io.DisplaySize = ImVec2(this->m_window.getWidth(), this->m_window.getHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Change imgui branch with Viewports
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        const auto backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace ogl
