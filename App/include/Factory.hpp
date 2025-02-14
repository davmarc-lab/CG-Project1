#pragma once

#include "Enemy.hpp"
#include "Entity.hpp"
#include "MultiShape.hpp"

struct BasicInfo {
    glm::vec3 position{};
    glm::vec3 scale{1, 1, 1};
    glm::vec3 rotation{};
};

std::vector<glm::vec4> getColorVector(const glm::vec4 &color, const size_t &size);

Shared<Entity> createSquare(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color = {1, 0, 0, 1});
Shared<Entity> createCircle(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const Pair<float> &center, const Pair<float> &radius,
                            const unsigned int &numTriangles, const glm::vec4 &color = {1, 0, 0, 1});
Shared<Entity> createTriangle(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color = {1, 0, 0, 1});

Shared<Entity> createProjectile(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color = {1, 1, 1, 1});

Shared<MultiShape> createEnemy(const ogl::ShaderProgram& shader, const EnemyType &type, const BasicInfo &info);
