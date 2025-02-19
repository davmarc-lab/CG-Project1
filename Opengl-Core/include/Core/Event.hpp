#pragma once

#include "Utils.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ogl {
	using EventType = std::string;

	class Event {
	public:
		inline EventType getType() const { return this->m_name; }

		Event() = delete;

		Event(const EventType &name) :
			m_name(std::move(name)) {}

		~Event() = default;

	private:
		EventType m_name{};
	};

	class EventManager {
	public:
		void post(const Event &event) const;

		void subscribe(const Event &event, std::function<void()> &&func);

		EventManager(EventManager &other) = delete;

		void operator=(const EventManager &other) = delete;

		inline static Shared<EventManager> instance() {
			if (s_pointer == nullptr) {
				Shared<EventManager> copy(new EventManager());
				copy.swap(s_pointer);
			}
			return s_pointer;
		}

		~EventManager() = default;

	private:
		EventManager() = default;

		inline static Shared<EventManager> s_pointer = nullptr;

		std::unordered_map<EventType, std::vector<std::function<void()>>> m_events{};
	};

	namespace event {
		namespace window {
			const Event WINDOW_CLOSE{"Window Close"};
			const Event WINDOW_RESIZE{"Window Resize"};
			const Event WINDOW_MINIMIZED{"Window Minimized"};
			const Event WINDOW_MAXIMIZED{"Window Maximized"};
			const Event WINDOW_FULLSCREEN{"Window Fullscreen"};
		} // namespace window

		namespace loop {
			const Event LOOP_INPUT{"Loop Input"};
			const Event LOOP_UPDATE{"Loop Update"};
			const Event LOOP_BEGIN_RENDER{"Loop Begin Render"};
			const Event LOOP_RENDER{"Loop Render"};
			const Event LOOP_END_RENDER{"Loop End Render"};
			const Event LOOP_RUN{"Loop Run"};
			const Event LOOP_PAUSE{"Loop Pause"};
			const Event LOOP_EXIT{"Loop Exit"};
		} // namespace loop

		namespace shader {
			const Event SHADER_PROJECTION_CHANGED = Event("Shader Projection Changed");
			const Event INIT_DEFAULT_SHADER = Event("Initialize Default Shader");
		} // namespace shader
	} // namespace event
} // namespace ogl
