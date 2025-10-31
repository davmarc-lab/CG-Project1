#pragma once

#include "Utils.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ogl {
	using EventType = std::string;

    /**
	 * @brief Implementation of an event.
	 *
	 * @note Every different event must have different description.
	 * They use description to filter all the events.
	*/
	class Event {
	public:
        /**
        * @brief Retrieves event type.
        *
        * @return the event type
        */
		inline EventType getType() const { return this->m_name; }

		Event() = delete;

        /**
        * @brief Instances an event with the given name.
        *
        * @param name
        */
		Event(const EventType &name) :
			m_name(std::move(name)) {}

		~Event() = default;

	private:
        /// event name
		EventType m_name{};
	};

    /**
    * @brief This class manages and dispatch all the events.
    * Each event registerd has a list of callbacks that are called
    * when the specific event is dispatched.
    */
	class EventManager {
	public:
        /**
        * @brief This method dispatch the given event and execute all the callbacks.
        *
        * @param event the event to dispatch
        */
		void post(const Event &event) const;

        /**
        * @brief Register a callback for the given event.
        *
        * @param event the event to register the callback
        * @param func the callback to be dispatched
        *
        * @see post()
        */
		void subscribe(const Event &event, std::function<void()> &&func);

        /**
        * @brief This method clears all registered events and callbacks.
        */
		void cleanAll();

		EventManager(EventManager &other) = delete;

		void operator=(const EventManager &other) = delete;

        /**
        * @brief Retrieves the instance of the EventManager.
        * If it's not instanced, it will be instanced automatically.
        *
        * @return a Shared<EventManager> object
        */
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

        /// static shared pointer for Singleton
		inline static Shared<EventManager> s_pointer = nullptr;
        /// map where callbacks are stored
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

		namespace render {
			const Event PREPARE_INSTANCED = Event("Prepare Instanced Mesh");
		}
	} // namespace event
} // namespace ogl
