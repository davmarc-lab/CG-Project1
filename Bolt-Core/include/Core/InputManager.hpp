#pragma once

#include "Utils.hpp"

#include <array>
#include <mutex>
#include <vector>

BT_CONSTEXPR u16 MAX_KEY_QUEUE = 20;
BT_CONSTEXPR u16 MAX_MOUSE_KEY_QUEUE = 8;

struct Key {
	u32 key;
	u32 status;
};

namespace bolt {
	class InputManager {
	public:
		InputManager(InputManager &other) = delete;

		void operator=(const InputManager &other) = delete;

		/*
		 * Retrieves the instance of the InputManager if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `InputManager` unique object.
		 */
		inline static Shared<InputManager> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<InputManager> copy(new InputManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		void keyPressed(const u32 &key);

		void keyReleased(const u32 &key);

		b8 isKeyPressed(const u32 &key) const;

		Key getKeyStatus(const u32 &key) const;

		std::vector<Key> getKeys();

	private:
		inline static Shared<InputManager> s_pointer = nullptr;
		inline static std::mutex s_mutex{};

		std::vector<Key> m_keys = std::vector<Key>();
		std::array<Key, MAX_MOUSE_KEY_QUEUE> m_mouse{};

		InputManager() = default;
	};
} // namespace bolt
