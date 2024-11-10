#include "../../include/Platform/ImGuiFactory.hpp"

#include "../../../Bolt-Vendor/include/glfw/include/GLFW/glfw3.h"

#include "../../../Bolt-imgui/include/imgui/imgui.h"

#include "../../include/Graphic/ImGuiLayer.hpp"
#include "../../include/Graphic/Texture/Texture.hpp"

#include <iostream>

namespace bolt {
	auto em = EntityManager::instance();

	void ImGuiDockSpace::onEvent(const Event &e) {}

	void ImGuiDockSpace::onAttach() {
		this->m_debugWindow = ImGuiDebug("Debug");
		this->m_attached = true;
	}

	void ImGuiDockSpace::onRender() {
		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport *viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &this->m_open, windowFlags);

		// Submit the DockSpace
		if (const ImGuiIO &io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			const ImGuiID &dockspaceId = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		static bool opt_fullscreen = false;
		static bool opt_padding = false;

		if (ImGui::BeginMenuBar()) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
			if (ImGui::BeginMenu("Options")) {
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Style")) {
					this->m_styleConfig = !this->m_styleConfig;
				}
				ImGui::EndMenu();
			}
			ImGui::PopStyleVar();

			if (ImGui::Button("Metrics")) {
				this->m_debugWindow.setVisible(!this->m_debugWindow.isOpened());
			}

			ImGui::EndMenuBar();
		}

		if (this->m_styleConfig) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
			ImGui::Begin("Style Editor", &this->m_styleConfig);
			ImGui::ShowStyleEditor(&ImGui::GetStyle());
			ImGui::End();
			ImGui::PopStyleVar();
		}

		if (this->m_debugWindow.isOpened()) {
			this->m_debugWindow.onRender();
		}

		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);
	}

	ImGuiEntityTree::ImGuiEntityTree() :
		m_entities(EntityManager::instance()->getEntitiesCount()) {
		if (!Application::isImGuiEnabled()) // PUT LOG HERE
			throw std::runtime_error("ImGui is disabled.");
	}

	void ImGuiEntityTree::onEvent(const Event &e) {}

	void ImGuiEntityTree::onRender() {
		ImGui::Begin(this->m_name.c_str());
		if (this->m_entities != EntityManager::instance()->getEntitiesCount()) {
			this->m_entities = EntityManager::instance()->getEntitiesCount();
		}
		for (auto id : EntityManager::instance()->getEntitiesId()) {
			if (ImGui::TreeNode(em->getEntityName(id).c_str())) {
				auto comp = em->getEntityComponent<Transform>(id);
				auto physic = em->getEntityComponent<PhysicComponent>(id);

				ImGui::PushID(&id);
				ImGui::Text("Name: %s", em->getEntityName(id).c_str());

				if (comp != nullptr) {
					auto pos = comp->getPosition();
					if (ImGui::DragFloat3("Position", &pos.x)) {
						comp->setPosition(pos);
					}
				}

				if (physic != nullptr) {
					auto mass = physic->mass;
					if (ImGui::InputFloat("Mass", &mass)) {
						physic->mass = mass;
					}
				}

				ImGui::PopID();
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void ImGuiViewPort::onAttach() {
		// preparing framebuffer
		// scene::perspectiveProjection = perspective(45.0f, static_cast<f32>(this->m_size.x) / this->m_size.y, 0.1f, 100.f);;
		// scene::orthoProjection = ortho(0.f, static_cast<f32>(this->m_size.x), 0.f, static_cast<f32>(this->m_size.y));
		this->m_attached = true;
	}

	void ImGuiViewPort::onEvent(const Event &e) {}

	void ImGuiViewPort::rescaleViewport(const u16 &width, const u16 &height) {}

	void ImGuiViewPort::bindFBO() { this->m_fbo.bind(); }

	void ImGuiViewPort::unbindFBO() { this->m_fbo.unbind(); }

	void ImGuiViewPort::onRender() {
		if (!this->m_fboReady) {
			ImGui::Begin(this->m_name.c_str());

			this->m_fboReady = true;
			const auto dim = ImGui::GetContentRegionAvail();
			this->m_size.x = static_cast<u16>(dim.x);
			this->m_size.y = static_cast<u16>(dim.y);

			switch (Application::getSceneType()) {
				case scene::SceneType::SCENE_2D:
					scene::updateOrtho(0.f, static_cast<f32>(this->m_size.x), 0.f, static_cast<f32>(this->m_size.y));
					break;
				case scene::SceneType::SCENE_3D:
					scene::updatePerspective(45.0f, static_cast<f32>(this->m_size.x) / this->m_size.y, 0.1f, 100.f);
					break;
			}
			scene::updateTextProj(0.f, this->m_size.x, 0.f, this->m_size.y);

			EventDispatcher::instance()->post(events::shader::ShaderProjectionChanged);

			this->m_fbo = FrameBuffer({{this->m_size}, fbo::Operation::FB_DEFAULT});
			this->m_fbo.onAttach();

			ImGui::End();
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		ImGui::Begin(this->m_name.c_str());
		ImGui::BeginChild("Render");

		auto dim = ImGui::GetContentRegionAvail();
		if (dim.x != this->m_size.x || dim.y != this->m_size.y) {
			this->m_fbo.rescaleFrameBuffer(static_cast<u16>(dim.x), static_cast<u16>(dim.y));
			this->m_size.x = static_cast<u16>(dim.x);
			this->m_size.y = static_cast<u16>(dim.y);

			switch (Application::getSceneType()) {
				case scene::SceneType::SCENE_2D:
					scene::updateOrtho(0.f, this->m_size.x, 0.f, this->m_size.y);
					break;
				case scene::SceneType::SCENE_3D:
					scene::updatePerspective(45.0f, static_cast<f32>(this->m_size.x) / this->m_size.y, 0.1f, 100.f);
					break;
			}
			scene::updateTextProj(0.f, this->m_size.x, 0.f, this->m_size.y);

			EventDispatcher::instance()->post(events::shader::ShaderProjectionChanged);
		}

		ImGui::Image((void *)(intptr_t)this->m_fbo.getTextureId(), ImVec2(this->m_size.x, this->m_size.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::EndChild();
		ImGui::End();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ImGuiUtility::onEvent(const Event &e) {}

	void ImGuiUtility::onRender() {
		ImGui::Begin(this->m_name.c_str());
		if (ImGui::Button("New Entity")) {
			EventDispatcher::instance()->post(events::ecs::CreateMeshEvent);
		}
		ImGui::End();
	}

	void ImGuiProperties::onEvent(const Event &e) {}

	void ImGuiProperties::onRender() {
		ImGui::Begin(this->m_name.c_str());

		ImGui::End();
	}

	void ImGuiFactory::createBasicUi() {
		LayerManager::instance()->addLayer(CreateShared<ImGuiDockSpace>());
		LayerManager::instance()->addLayer(CreateShared<ImGuiEntityTree>());
		LayerManager::instance()->addLayer(CreateShared<ImGuiViewPort>());
		LayerManager::instance()->addLayer(CreateShared<ImGuiUtility>());
		LayerManager::instance()->addLayer(CreateShared<ImGuiProperties>());
	}
} // namespace bolt
