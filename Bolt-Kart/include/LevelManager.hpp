#pragma once

#include "../../Bolt-Core/include/Engine.hpp"
#include "Utils.hpp"

#include <mutex>


struct BulletColor {
    bolt::vec4 base{0, 0, 0, 1};
	DoubleColor peak{{0, 0, 0, 1}, {0, 0, 0, 1}};
};

class LevelManager {
public:
    void createLevel();

    void cleanLevel();

    inline u32 getCurrentLevel() const { return this->m_current; }

    BulletColor getBulletColors() const { return this->m_settings; }

    void setBulletColors(const BulletColor& colors) { this->m_settings = colors; }

	LevelManager(LevelManager &other) = delete;

	void operator=(const LevelManager &other) = delete;

	/*
	 * Retrieves the instance of the LevelManager if it's not created.
	 * This function is thread safe using a simple `std::mutex`.
	 *
	 * @return `LevelManager` unique object.
	 */
	inline static Shared<LevelManager> instance() {
		std::lock_guard<std::mutex> lock(s_mutex);
		if (s_pointer == nullptr) {
			Shared<LevelManager> copy(new LevelManager());
		copy.swap(s_pointer);
		}

		return s_pointer;
	}

private:
	inline static Shared<LevelManager> s_pointer;
	inline static std::mutex s_mutex;

	u32 m_current = 10;
    BulletColor m_settings{};

    MultiMesh createBullet(const bolt::vec2& pos);

	LevelManager() = default;
};
