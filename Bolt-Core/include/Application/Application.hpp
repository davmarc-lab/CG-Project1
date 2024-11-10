#pragma once

#include "../Core/Scene.hpp"
#include "../Core/Structs.hpp"

namespace bolt {
	struct CullFace {
		b8 enabled = false;
		u32 mode = 0;
	};

	struct DepthTest {
		b8 enabled = false;
		b8 mask = true;
		u32 func = 0;
	};

	struct WindowProperties {
		b8 maximized = false;
		b8 vsync = false;
		vec4 backgroundColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
		CullFace cull = {};
		DepthTest depth = {};
	};

	struct ApplicationSetting {
		scene::SceneType type;
		std::string name = "Bolt App";
		Pair<u16> dimension = {1600, 900};
		Pair<u16> position = {};
		WindowProperties properties;
		b8 enableCollisions = false;
		b8 defaultCameraMovement = false;
	};

	/*
	 * Basic entry point to use the engine.
	 */
	class Application {
	private:
		inline static b8 s_enableImGui = false;

		inline static b8 s_close = false;

		inline static ApplicationSetting s_settings{};

		inline static mat4 s_projection{};
		inline static mat4 s_textProjection{};

	public:
		Application() :
			Application({scene::SceneType::SCENE_3D}) {}

		Application(const ApplicationSetting &settings) {
			s_settings = settings;
		}

		/*
		 * Starts the aplication, and enters in the loop.
		 */
		void run();

		inline static void enableImGui() { s_enableImGui = true; }

		inline static bool isImGuiEnabled() { return s_enableImGui; }

		inline static scene::SceneType getSceneType() { return s_settings.type; }

		inline static mat4 getProjectionMatrix() { return s_projection; }

		inline static void setProjectionMatrix(const mat4 &proj) { s_projection = proj; }

		inline static mat4 getTextProjMatrix() { return s_textProjection; }

		inline static void setTextProjMatrix(const mat4 &proj) { s_textProjection = proj; }

		inline void closeApplication() { s_close = true; }

		inline b8 shouldClose() const { return s_close; }

		~Application() = default;
	};
} // namespace bolt
