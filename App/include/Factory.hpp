#pragma once

#include "Enemy.hpp"
#include "Entity.hpp"
#include "MultiShape.hpp"
#include "Projectile.hpp"

struct BasicInfo {
	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
};

std::vector<glm::vec4> getColorVector(const glm::vec4 &color, const size_t &size);

Shared<MultiShape> createEnemy(const ogl::ShaderProgram &shader, const EnemyType &type, const BasicInfo &info);

unsigned int factorySquare(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const EnemyInfo &stats = {});

unsigned int factoryCircle(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const unsigned int &numTriangles = 20, const Pair<float> &center = {0, 0}, const Pair<float> &radius = {1, 1}, const EnemyInfo &stats = {});

unsigned int factoryProjectile(const BasicInfo &info, const glm::vec4 &color, const ProjInfo &projInfo, const glm::vec3 &direction);

unsigned int factoryHermite(const BasicInfo &info, const std::string &path, const glm::vec4 &color = {1, 0, 0, 1});

unsigned int factoryEnemy(const BasicInfo &info, const glm::vec4 &color);
