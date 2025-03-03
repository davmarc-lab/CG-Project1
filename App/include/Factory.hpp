#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct BasicInfo {
	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
};

struct EntityVertex {
	EntityVertex() = default;

	EntityVertex(const std::vector<glm::vec3> &coords, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices) :
		coords(coords), colors(colors), indices(indices) {}

	std::vector<glm::vec3> coords{};
	std::vector<glm::vec4> colors{};
	std::vector<unsigned int> indices{};
};

const glm::vec3 ENEMY_SLIME_SIZE{120, 220, 1};
const glm::vec3 SLIME_LEYE_OFFSET{-13, 0, 0};
const glm::vec3 SLIME_REYE_OFFSET{10, 0, 0};
const glm::vec3 SLIME_EYE_SCALE{10, 15, 1};

struct EnemyInfo {
	float lastShoot = 0;
	float cooldown = 1;
	float damage = 20;
	float hp = 100;
	float range = 400;
};

const EnemyInfo ENEMY_FLY_INFO{0, 2, 30, 50, 600};
const EnemyInfo ENEMY_SLIME_INFO{0, 1, 10, 100, 600};

const glm::vec3 PROJ_SIZE = {10, 10, 1};
const glm::vec4 PROJ_COLOR = {0, 1, 0, 1};
const float PROJ_VEL = 5;
const float PROJ_RANGE = 400.f;

struct ProjInfo {
	float damage = 20;
	float range = PROJ_RANGE;
	glm::vec4 color = PROJ_COLOR;
};

std::vector<glm::vec4> getColorVector(const glm::vec4 &color, const size_t &size);

unsigned int factoryBackground(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

unsigned int factorySquare(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const EnemyInfo &stats = {});

unsigned int factoryCircle(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const unsigned int &numTriangles = 20, const Pair<float> &center = {0, 0}, const Pair<float> &radius = {1, 1}, const EnemyInfo &stats = {});

unsigned int factoryProjectile(const BasicInfo &info, const glm::vec4 &color, const ProjInfo &projInfo, const glm::vec3 &direction);

unsigned int factoryHermite(const BasicInfo &info, const std::string &path, const glm::vec4 &color = {1, 0, 0, 1});

unsigned int factoryEnemy(const BasicInfo &info, const glm::vec4 &color);
