#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>

#include "Utils.hpp"

namespace bolt {
	/*
	 * Implementation of standard custom events.
	 *
	 * Every different event must have different description.
	 * They use description to filter all the events.
	 */
	class Event {
	public:
		using EventType = std::string;

		explicit Event(std::string name = "Basic Event") : m_name(std::move(name)) {}

		b8 operator==(const Event& other) const { return this->m_name == other.getType(); }

		~Event() = default;

		/*
		 * Retrieves the event type (description).
		 *
		 * @return event description `std::string`.
		 */
		const EventType& getType() const { return this->m_name; }

	private:
		EventType m_name;
	};

	namespace events {
		namespace window {
			// Window events
			BT_AUTOCONST WindowCreatedEvent = Event("Window Created");
			BT_AUTOCONST WindowDestroyedEvent = Event("Window Destroyed");
			BT_AUTOCONST WindowCloseEvent = Event("Window Close");
		} // namespace window

		namespace input {
			// Input events
			BT_AUTOCONST KeyPressedEvent = Event("Key Pressed");
			BT_AUTOCONST KeyReleasedEvent = Event("Key Released");
			BT_AUTOCONST KeyModPressedEvent = Event("Mod Key Pressed");
			BT_AUTOCONST KeyModReleasedEvent = Event("Mod Key Released");

			BT_AUTOCONST MouseMoveEvent = Event("Mouse Moved");
			BT_AUTOCONST MouseStopEvent = Event("Mouse Stopped");
			BT_AUTOCONST MouseScrollUpEvent = Event("Mouse Scroll Up");
			BT_AUTOCONST MouseScrollDownEvent = Event("Mouse Scroll Down");

			BT_AUTOCONST MouseLeftButtonPressedEvent = Event("Mouse LButton Pressed");
			BT_AUTOCONST MouseLeftButtonReleasedEvent = Event("Mouse LButton Pressed");
			BT_AUTOCONST MouseRightButtonPressedEvent = Event("Mouse RButton Pressed");
			BT_AUTOCONST MouseRightButtonReleasedEvent = Event("Mouse RButton Pressed");
			BT_AUTOCONST MouseMiddleButtonPressedEvent = Event("Mouse MButton Pressed");
			BT_AUTOCONST MouseMiddleButtonReleasedEvent = Event("Mouse MButton Pressed");
		} // namespace input

		namespace ecs {
			BT_AUTOCONST EntityCreatedEvent = Event("Entity Created");
			BT_AUTOCONST EntityRemovedEvent = Event("Entity Removed");
			BT_AUTOCONST CreateMeshEvent = Event("Create Mesh");
			BT_AUTOCONST ComponentAttachedToEntityEvent = Event("Component Attached");
			BT_AUTOCONST ComponentDetachFromEntityEvent = Event("Component Detached");
		} // namespace ecs

		namespace loop {
			BT_AUTOCONST LoopInput = Event("Input");
			BT_AUTOCONST LoopUpdate = Event("Update");
			BT_AUTOCONST LoopBeforeRender = Event("Before Render");
			BT_AUTOCONST LoopRender = Event("Render");
			BT_AUTOCONST LoopPauseEvent = Event("Loop Pause");
			BT_AUTOCONST LoopResumeEvent = Event("Loop Resume");
			BT_AUTOCONST LoopStopEvent = Event("Loop Stop");
			BT_AUTOCONST LoopGeneric = Event("Loop Generic");
		} // namespace loop

		namespace shader {
			BT_AUTOCONST ShaderProjectionChanged = Event("Changed Projection");
		} // namespace shader
	};    // namespace events

	/*
	 * Implementation of an Event manager using observers.
	 *
	 * This class use a thread safe sinleton to create only one instance of this object.
	 * It stores all the callbacks functions for each EventType, at some point when an
	 * event is dispatched (`EventDispatcher::post()`) all callbacks function are executed.
	 */
	class EventDispatcher {
	public:
		using EventCallback = std::function<void(const Event&)>;

		EventDispatcher(EventDispatcher& other) = delete;

		void operator=(const EventDispatcher& other) = delete;

		/*
		 * Retrieves the instance of the EventDispatcher if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `EventDispatcher` unique object.
		 */
		inline static Shared<EventDispatcher> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<EventDispatcher> copy(new EventDispatcher());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		/*
		 * Stores each callback function for the `Event` given.
		 * It can sore multiple callback for the same `Event`.
		 *
		 * @param event the `Event` to be observed.
		 * @param callback the callback function to be executed.
		 */
		void subscribe(const Event& event, EventCallback&& callback);

		/*
		 * Dispatch the given `Event` and execute all the callbacks.
		 *
		 * @param event the `Event` to be dispatched.
		 */
		void post(const Event& event) const;

	private:
		inline static Shared<EventDispatcher> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		std::map<Event::EventType, std::vector<EventCallback>> m_observers;

		EventDispatcher() = default;
	};
} // namespace bolt
