#pragma once

#include "../Graphic.hpp"

#include "Layer.hpp"
#include "Structs.hpp"
#include "Utils.hpp"

#include <functional>

#include <string>

namespace ogl {

	struct WindowCallbacks {
		std::function<void(GLFWwindow *, int, int)> resizeCallback;
		std::function<void(GLFWwindow *, int, int, int, int)> keyCallback = nullptr;
		std::function<void(GLFWwindow *, int, int, int)> mouseButtonCallback = nullptr;
		std::function<void(GLFWwindow *, double, double)> cursorPosCallback = nullptr;
	};

	struct WindowSettings {
		std::string name = "GLFW Window";
		Pair<float> size{1600, 900};
		Pair<int> position{0, 0};

		bool maximized = false;
		bool fullscreen = false;
		bool focused = false;
		bool decorated = true;
		bool vsync = false;
		Color bgColor = {0.3f, 0.3f, 0.3f, 1.0f};
	};

	class Window : public Layer {
	public:
		Window(const WindowSettings &settings);
		Window() = delete;
		~Window() = default;

		inline GLFWwindow *getContext() const { return this->m_context; }

		inline float getWidth() const { return this->m_settings.size.x; }
		void setWidth(const float &val) { this->m_settings.size.x = val; }

		inline float getHeight() const { return this->m_settings.size.y; }
		void setHeight(const float &val) { this->m_settings.size.y = val; }

		void toggleVsync();
		bool isVsyncEnabled() const;

		void setResizeCallback(std::function<void(GLFWwindow *, int, int)> &&func);
		void execResizeCallback(GLFWwindow *context, const int &width, const int &height);

		void setKeysCallback(std::function<void(GLFWwindow *, int, int, int, int)> &&func);
		void execKeysCallback(GLFWwindow *context, const int &key, const int &code, const int &action, const int &mods);

		void setMouseButtonCallback(std::function<void(GLFWwindow *, int, int, int)> &&func);
		void execMouseButtonCallback(GLFWwindow *context, const int &button, const int &action, const int &mods);

		void setCursorPosCallback(std::function<void(GLFWwindow *, double, double)> &&func);
		void execCursorPosCallback(GLFWwindow *context, const double &xpos, const double &ypos);

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate() override;
		virtual void onRender() override;

		virtual void begin() override;
		virtual void end() override;

	private:
		GLFWwindow *m_context = nullptr;
		WindowSettings m_settings{};
		unsigned int m_clearMask = 0;

		WindowCallbacks m_callbacks{};

		void updateUserPointer();
	};

} // namespace ogl
