#include "../../include/Core/Event.hpp"

namespace ogl {

void EventManager::post(const Event &event) const {
    auto find = this->m_events.find(event.getType());
    if (find != this->m_events.end())
        for (auto &&f : find->second) {
            f();
        }
}

void EventManager::subscribe(const Event &event, std::function<void()> &&func) {
    if (this->m_events.find(event.getType()) == this->m_events.end()) {
        this->m_events.insert_or_assign(event.getType(), std::vector<std::function<void()>>{});
    }
    this->m_events.at(event.getType()).push_back(std::move(func));
}

} // namespace ogl
