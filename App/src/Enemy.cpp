#include "../include/Enemy.hpp"

Enemy::Enemy(const EnemyInfo &info, const EntityVertex &vertex, const EntityModel &model) : m_info(info), Entity(vertex, model) {
    this->setCollidable(true);
}
