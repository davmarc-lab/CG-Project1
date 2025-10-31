#pragma once

#include "Utils.hpp"

#include <array>
#include <mutex>
#include <vector>

const int MAX_KEY_QUEUE = 20;
const int MAX_MOUSE_KEY_QUEUE = 8;

/**
 * @brief Data structure used to wrap keyboard keys.
 */
struct Key {
    /// key code
	unsigned int key;
    /// key status code
	unsigned int status;
};

namespace ogl {

    /**
     * @brief This class is used to manage input from keyboard.
     * It can store all the key pressed while executing a `Window`.
     */
	class InputManager {
	public:
		InputManager(InputManager &other) = delete;

		void operator=(const InputManager &other) = delete;

		/**
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

        /**
        * @brief Sets the status of the given key to pressed.
        *
        * @param key key code
        */
		void keyPressed(const unsigned int &key);

        /**
        * @brief Sets the status of the given key to released.
        *
        * @param key key code
        */
		void keyReleased(const unsigned int &key);

        /**
        * @brief Retrieves the current state of the given key.
        *
        * @param key key code
        *
        * @return true if the given key is pressed, false if it's released
        */
		bool isKeyPressed(const unsigned int &key) const;

        /**
        * @brief Return the `Key` data structure of the given key.
        *
        * @param key key code
        *
        * @return the given key values
        */
		Key getKeyStatus(const unsigned int &key) const;

        /**
        * @brief Retrieves all the current keys pressed.
        *
        * @return an array of all pressed keys
        */
		std::vector<Key> getKeys();

	private:
	    /// static shared pointer for Singleton
		inline static Shared<InputManager> s_pointer = nullptr;
        /// static variable used for mutex purpose
		inline static std::mutex s_mutex{};

        /// vector with all keyboard keys pressed 
		std::vector<Key> m_keys = std::vector<Key>();
        /// array with all mouse keys pressed 
		std::array<Key, MAX_MOUSE_KEY_QUEUE> m_mouse{};

        /// Default constructor.
		InputManager() = default;
	};
} // namespace ogl
