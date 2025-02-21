#pragma once

#include "Entity.hpp"

#include <glm/glm.hpp>

const glm::vec3 PROJ_SIZE = {10, 10, 1};
const glm::vec4 PROJ_COLOR = {0, 1, 0, 1};
const float PROJ_VEL = 5;
const float PROJ_RANGE = 50.f;

struct ProjInfo {
	float damage{};
	float range = PROJ_RANGE;
	glm::vec4 color = PROJ_COLOR;
};

class Projectile : public Entity {
public:
	inline glm::vec3 getStartPoint() const { return this->m_startPos; }

	inline void setVelocity(const glm::vec3 &vel) { this->m_velocity = vel; }

	inline float getMaxRange() const { return this->m_info.range; }

	inline bool isDistanceLeft() const { return glm::length(this->getPosition() - this->m_startPos) > this->m_info.range; }

	inline void endLife() { this->m_dead = true; }

	inline bool isDead() const { return this->m_dead || this->isDistanceLeft(); }

	inline float getDamage() const { return this->m_info.damage; }

	Projectile() = delete;

	Projectile(const ProjInfo &info, const glm::vec3 pos, const ogl::ShaderProgram &shader, const glm::vec4 &color = PROJ_COLOR);

	virtual ~Projectile() = default;

private:
	ProjInfo m_info{};
	glm::vec4 m_color{};
	glm::vec3 m_velocity{};
	glm::vec3 m_startPos{};
	bool m_dead = false;
};
