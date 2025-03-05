#include "../include/LevelManager.hpp"
#include <GLFW/glfw3.h>

void LevelManager::incrementLevel() {
	this->clearLevel();
	for (auto i = 0; i < this->m_modifiers.numEnemiesWave; i++) {
		ogl::EventManager::instance()->post(EVENT_ENEMY_SPAWN);
		this->m_data.numEnemies++;
		this->m_data.enemiesSpawned++;
	}
	this->m_data.lastEnemySpawnTime = glfwGetTime();
	this->m_currentLevel++;
	ogl::EventManager::instance()->post(EVENT_LEVEL_COMPLETED);
}

void LevelManager::clearLevel() {
	this->m_data.enemiesSpawned = 0;
	this->m_data.numEnemies = 0;
	this->m_data.lastEnemySpawnTime = 0;
}

void LevelManager::onUpdate() {
	auto time = glfwGetTime();

	if (this->m_data.pauseTime > 0) {
		if (time - this->m_data.lastPauseTime > this->m_data.pauseTime) {
			this->m_data.pauseTime = 0;
		}
		return;
	}

	if (this->isLevelCompleted() || this->m_currentLevel == 0)
		this->incrementLevel();

	if (this->m_data.enemiesSpawned == this->m_modifiers.enemiesMaxNumber) {
		return;
	}

	if (this->m_data.numEnemies == 0) {
		ogl::EventManager::instance()->post(EVENT_ENEMY_SPAWN);
		this->m_data.numEnemies++;
		this->m_data.enemiesSpawned++;
		return;
	}

	if (time - this->m_data.lastEnemySpawnTime > this->m_modifiers.enemiesSpawnDelay) {
		ogl::EventManager::instance()->post(EVENT_ENEMY_SPAWN);
		this->m_data.lastEnemySpawnTime = time;
		this->m_data.numEnemies++;
		this->m_data.enemiesSpawned++;
	}
}
