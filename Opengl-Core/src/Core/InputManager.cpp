#include "../../include/Core/InputManager.hpp"

#include <algorithm>
#include <vector>

#include "../../include/Graphic.hpp"

namespace ogl {
	void InputManager::keyPressed(const unsigned int &key) {
		if (this->m_keys.size() < MAX_KEY_QUEUE) {
			if (!this->isKeyPressed(key))
				this->m_keys.push_back({key, GLFW_PRESS});
		}
	}

	void InputManager::keyReleased(const unsigned int &key) {
		auto val = std::find_if(this->m_keys.begin(), this->m_keys.end(), [&key](Key elem) { return elem.key == key; });
		this->m_keys.erase(val);
	}

	bool InputManager::isKeyPressed(const unsigned int &key) const {
		return std::find_if(this->m_keys.begin(), this->m_keys.end(), [&key](Key elem) {
				   return elem.key == key;
			   }) != this->m_keys.end();
	}
} // namespace ogl
