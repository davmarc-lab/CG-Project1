#include "../include/AppGui.hpp"

#include "../include/ECS/Ett.hpp"

void ImGuiStats::onRender() {
	ImGui::Begin("Stats");
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
	ImGui::Text("Delta Time: %f", ImGui::GetIO().DeltaTime);
	ImGui::Text("(ECS) Entities Count: %u", EntityManager::instance()->getEntitiesCount());
	ImGui::End();
}
