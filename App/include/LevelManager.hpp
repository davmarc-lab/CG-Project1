#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#define MAX_POSSIBLE_DELAY 3
#define MAX_POSSIBLE_ENEMIES 10

const auto EVENT_LEVEL_COMPLETED = ogl::Event("Level Completed");
const auto EVENT_SPAWN_ITEM = ogl::Event("Spawn Item");
const auto EVENT_SPAWN_HEALTH = ogl::Event("Spawn Health Item");
const auto EVENT_SPAWN_POWERUP = ogl::Event("Spawn Powerup Item");
const auto EVENT_ENEMY_SPAWN = ogl::Event("Spawn Enemy Event");
const auto EVENT_ENEMY_DEAD = ogl::Event("Dead Enemy Event");

struct LevelModifiers {
	unsigned int enemiesMaxNumber = 5;
	float enemiesSpawnDelay = 5.f;
	unsigned int numEnemiesWave = 3;
};

struct LevelData {
	float pauseTime = 0.f;
	float lastPauseTime = 0.f;
	unsigned int numEnemies = 0;
	unsigned int enemiesSpawned = 0;
	float lastEnemySpawnTime = 0;
};

class LevelManager {
public:
	void incrementLevel();

	bool isLevelCompleted() const { return this->m_data.enemiesSpawned == this->m_modifiers.enemiesMaxNumber && this->m_data.numEnemies == 0; }

	void clearLevel();

	void onUpdate();

	inline unsigned int getMaxEnemies() const { return this->m_modifiers.enemiesMaxNumber; }

	inline unsigned int getEnemiesSpawned() const { return this->m_data.enemiesSpawned; }

	inline unsigned int getCurrentEnemies() const { return this->m_data.numEnemies; }

	inline unsigned int getCurrentLevel() const { return this->m_currentLevel; }

	inline void setPause(const float &time) {
		this->m_data.pauseTime = time;
		this->m_data.lastPauseTime = glfwGetTime();
	}

	inline void decreaseEnemies(const unsigned int val = 1) { this->m_data.numEnemies -= val; }

	LevelManager() = default;

	~LevelManager() = default;

private:
	unsigned int m_currentLevel = 0;
	LevelData m_data{};
	LevelModifiers m_modifiers{};
};
