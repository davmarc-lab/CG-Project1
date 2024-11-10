#include "../../include/Core/Event.hpp"

namespace bolt {
	void EventDispatcher::subscribe(const Event& event, EventCallback&& callback) {
		if (!this->m_observers.contains(event.getType()))
			this->m_observers.emplace(event.getType(), std::vector<EventCallback>{});
		this->m_observers.at(event.getType()).emplace_back(std::move(callback));
	}

	void EventDispatcher::post(const Event& event) const {
		if (!this->m_observers.contains(event.getType())) { return; }

		auto&& observers = this->m_observers.at(event.getType());
		for (auto&& o : observers) {
			o(event);
		}
	}
}
