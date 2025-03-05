#include "../../include/Core/Window.hpp"

#include "../../include/Graphic.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

namespace ogl {
	// error callback
	static void errorCallback(int code, const char *description) { std::cerr << "GLFW error (" << code << ") -> (" << description << ")\n"; }

	// resize Callback
	static void resizeCallback(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);

		auto pt = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if (pt != nullptr) {
			pt->execResizeCallback(window, width, height);

			pt->setWidth(width);
			pt->setHeight(height);
		}
	}

	// keyboard input callback
	static void keyboardCallback(GLFWwindow *window, int key, int code, int action, int mod) {
		auto pt = glfwGetWindowUserPointer(window);
		if (pt != nullptr) {
			static_cast<Window *>(pt)->execKeysCallback(window, key, code, action, mod);
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	// mouse button callback
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
		auto pt = glfwGetWindowUserPointer(window);
		if (pt != nullptr) {
			static_cast<Window *>(pt)->execMouseButtonCallback(window, button, action, mods);
		}
	}

	// cursor position callback
	static void cursorPosCallback(GLFWwindow *window, double x, double y) {
		auto pt = glfwGetWindowUserPointer(window);
		if (pt != nullptr) {
			static_cast<Window *>(pt)->execCursorPosCallback(window, x, y);
		}
	}

	Window::Window(const WindowSettings &settings) :
		Layer("GLFW Window"), m_settings(settings) {}

	void Window::toggleVsync() {
		this->m_settings.vsync = !this->m_settings.vsync;
		glfwSwapInterval(this->m_settings.vsync ? 1 : 0);
	}

	bool Window::isVsyncEnabled() const { return this->m_settings.vsync; }

	void Window::setResizeCallback(std::function<void(GLFWwindow *, int, int)> &&func) {
		this->m_callbacks.resizeCallback = std::move(func);
		this->updateUserPointer();
	}

	void Window::execResizeCallback(GLFWwindow *window, const int &width, const int &height) {
		if (this->m_callbacks.resizeCallback != nullptr)
			this->m_callbacks.resizeCallback(window, width, height);
	}

	void Window::setKeysCallback(std::function<void(GLFWwindow *, int, int, int, int)> &&func) {
		this->m_callbacks.keyCallback = std::move(func);
		this->updateUserPointer();
	}

	void Window::execKeysCallback(GLFWwindow *context, const int &key, const int &code, const int &action, const int &mods) {
		if (this->m_callbacks.keyCallback != nullptr)
			this->m_callbacks.keyCallback(context, key, code, action, mods);
	}

	void Window::setMouseButtonCallback(std::function<void(GLFWwindow *, int, int, int)> &&func) {
		this->m_callbacks.mouseButtonCallback = std::move(func);
		this->updateUserPointer();
	}

	void Window::execMouseButtonCallback(GLFWwindow *context, const int &button, const int &action, const int &mods) {
		if (this->m_callbacks.mouseButtonCallback != nullptr)
			this->m_callbacks.mouseButtonCallback(context, button, action, mods);
	}

	void Window::setCursorPosCallback(std::function<void(GLFWwindow *, double, double)> &&func) {
		this->m_callbacks.cursorPosCallback = std::move(func);
		this->updateUserPointer();
	}

	void Window::execCursorPosCallback(GLFWwindow *context, const double &xpos, const double &ypos) {
		if (this->m_callbacks.cursorPosCallback != nullptr)
			this->m_callbacks.cursorPosCallback(context, xpos, ypos);
	}

	void Window::updateUserPointer() { glfwSetWindowUserPointer(this->m_context, this); }

	void Window::onAttach() {
		if (this->m_attached)
			return;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwSetErrorCallback(errorCallback);

		this->m_context = glfwCreateWindow(this->m_settings.size.x, this->m_settings.size.y, this->m_settings.name.c_str(),
										   this->m_settings.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		ASSERT(this->m_context != nullptr);

		glfwMakeContextCurrent(this->m_context);

		glfwSetWindowUserPointer(this->m_context, this);

		// custom parameters
		if (this->m_settings.maximized)
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		if (this->m_settings.focused)
			glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

		if (this->m_settings.decorated)
			glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

		if (this->m_settings.vsync)
			glfwSwapInterval(1);

		// icon???

		// init glad for this context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			/* BT_ERROR_CORE("Failed to initialize GLAD."); */
			glfwTerminate();
			/* BT_INFO_CORE("To be replaced with event."); */
			exit(EXIT_FAILURE);
		}

		// resize callback
		glfwSetFramebufferSizeCallback(this->m_context, resizeCallback);

		// keyboard callback
		glfwSetKeyCallback(this->m_context, keyboardCallback);
		// mouse button callback
		glfwSetMouseButtonCallback(this->m_context, mouseButtonCallback);
		// cursor position callback
		glfwSetCursorPosCallback(this->m_context, cursorPosCallback);

		// error callback

		// debug callback

		// enable blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		this->m_clearMask |= GL_COLOR_BUFFER_BIT;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		this->m_clearMask |= GL_DEPTH_BUFFER_BIT;

		this->m_attached = true;
	}

	void Window::onDetach() { glfwDestroyWindow(this->m_context); }

	void Window::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(this->m_context);
	}

	void Window::onRender() {
		glClearColor(this->m_settings.bgColor.r, this->m_settings.bgColor.g, this->m_settings.bgColor.b, this->m_settings.bgColor.a);
		glClear(this->m_clearMask);
	}

	void Window::begin() {}

	void Window::end() {}
} // namespace ogl
