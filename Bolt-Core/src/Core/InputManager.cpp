#include "../../include/Core/InputManager.hpp"

#include <algorithm>
#include <vector>

#include "../../include/Graphics.hpp"

namespace bolt {
	void InputManager::keyPressed(const u32 &key) {
		if (this->m_keys.size() < MAX_KEY_QUEUE) {
			if (!this->isKeyPressed(key))
				this->m_keys.push_back({key, GLFW_PRESS});
		}
	}

	void InputManager::keyReleased(const u32 &key) {
		auto val = std::find_if(this->m_keys.begin(), this->m_keys.end(), [&key](Key elem) { return elem.key == key; });
		this->m_keys.erase(val);
	}

	b8 InputManager::isKeyPressed(const u32 &key) const {
		return std::find_if(this->m_keys.begin(), this->m_keys.end(), [&key](Key elem) {
				   return elem.key == key;
			   }) != this->m_keys.end();
	}
} // namespace bolt
