#pragma once

#include <stdexcept>
#include "../Core/Utils.hpp"

#include "../Core/LayerManager.hpp"
#include "../ECS/EntityManager.hpp"

#include "../Graphic/ImGui/ImGuiDebug.hpp"

#include "../Graphic/Buffer/FrameBuffer.hpp"

#include "../Application/Application.hpp"

namespace bolt {
	class ImGuiDockSpace : public Layer {
	private:
		std::string m_name = "DockSpace";
		b8 m_open = true;

		// other windows
		b8 m_styleConfig = false;

		ImGuiDebug m_debugWindow;

	public:
		ImGuiDockSpace() {
			if (!Application::isImGuiEnabled()) // PUT LOG HERE
				throw std::runtime_error("ImGui is disabled.");
		}

		virtual ~ImGuiDockSpace() override = default;

		virtual void onAttach() override;

		virtual void onEvent(const Event& e) override;

		virtual void onRender() override;
	};

	class ImGuiEntityTree : public Layer {
	private:
		std::string m_name = "Entities";
		u32 m_entities;

	public:
		ImGuiEntityTree();

		~ImGuiEntityTree() override = default;

		virtual void onEvent(const Event& e) override;

		virtual void onRender() override;
	};

	class ImGuiViewPort : public Layer {
	private:
		std::string m_name = "Viewport";
		Pair<u16> m_size;
		FrameBuffer m_fbo;
		b8 m_fboReady = false;
		u32 fbo = 0;
		u32 rbo = 0;
		u32 text = 0;

		void bindFBO();

		void unbindFBO();

	public:
		ImGuiViewPort() {
			if (!Application::isImGuiEnabled()) // PUT LOG HERE
				throw std::runtime_error("ImGui is disabled.");
		}

		~ImGuiViewPort() override = default;

		void rescaleViewport(const u16& width, const u16& height);

		virtual void begin() override { this->bindFBO(); }

		virtual void end() override { this->unbindFBO(); }

		virtual void onAttach() override;

		virtual void onEvent(const Event& e) override;

		virtual void onRender() override;
	};

	class ImGuiUtility : public Layer {
	private:
		std::string m_name = "Utility";

	public:
		ImGuiUtility() {
			if (!Application::isImGuiEnabled()) // PUT LOG HERE
				throw std::runtime_error("ImGui is disabled.");
		}

		~ImGuiUtility() override = default;

		virtual void onEvent(const Event& e) override;

		virtual void onRender() override;
	};

	class ImGuiProperties : public Layer {
	private:
		std::string m_name = "Property";

	public:
		ImGuiProperties() {
			if (!Application::isImGuiEnabled()) // PUT LOG HERE
				throw std::runtime_error("ImGui is disabled.");
		}

		~ImGuiProperties() override = default;

		virtual void onEvent(const Event& e) override;

		virtual void onRender() override;
	};

	class ImGuiFactory {
		ImGuiFactory() = delete;

	public:
		static void createBasicUi();
	};

	inline void addFullDockspaceToLayerStack() {
		Application::enableImGui();
		ImGuiFactory::createBasicUi();
	}
} // namespace bolt
