#pragma once

#include "../Application/Application.hpp"
#include "../Core/Structs.hpp"
#include "../Core/Utils.hpp"

#include "../Core/Event.hpp"

#include "../Core/Layer.hpp"

namespace bolt {
	class Window : public Layer {
	protected:
		Pair<u16> m_position{};
		Pair<u16> m_size{};

		b8 m_vsync = false;
		vec4 m_clearColor = vec4(0, 0, 0, 1);

		ApplicationSetting m_settings{};

		void *m_window = nullptr;
		std::string m_windowTitle = "Glfw Window";
        u32 m_clearMask = 0;

	public:
		Window(const ApplicationSetting &settings) :
			Layer("Glfw Window"), m_windowTitle(std::move(settings.name)) {
			this->m_position = settings.position;
			this->m_size = settings.dimension;
			this->m_vsync = settings.properties.vsync;
			this->m_clearColor = settings.properties.backgroundColor;

			this->m_settings = settings;

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

		virtual ~Window() = default;

		inline const u16 &getWidth() const { return this->m_size.x; }

		inline const u16 &getHeight() const { return this->m_size.y; }

		inline void setSize(const Pair<u16> &size) { this->m_size = size; }

		inline const Pair<u16> &getPosition() const { return this->m_position; }

		inline void setPosition(const Pair<u16> &position) { this->m_position = position; }

		virtual b8 shouldWindowClose() const;

		virtual void setVsync(const bool &vsync);

		inline const b8 &isVerticalSyncEnable() const { return this->m_vsync; }

		inline void *getCurrentWindow() const { return this->m_window; }

		inline vec4 getClearColor() const { return this->m_clearColor; }

		inline void setClearColor(const vec4 &color) { this->m_clearColor = color; }

		// --- Layer ---
		virtual void onAttach() override;

		virtual void onDetach() override;

		virtual void onEvent(const Event &e) override;

		virtual void onUpdate() override;

		virtual void onRender() override;

		virtual void end() override;
	};
} // namespace bolt
