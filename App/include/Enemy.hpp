#pragma once

#include "Entity.hpp"
#include <functional>

struct EnemyInfo {
    float lastShoot = 0;
    float cooldown = 1;
    float damage = 20;
    float hp = 100;
    float range = 400;
};

enum EnemyType { EMENY_SLIME, ENEMY_FLY };

const EnemyInfo ENEMY_SLIME_INFO{0, 1, 10, 100, 600};
const glm::vec3 ENEMY_SLIME_SIZE{120, 220, 1};
const glm::vec3 SLIME_LEYE_OFFSET{-13, 0, 0};
const glm::vec3 SLIME_REYE_OFFSET{10, 0, 0};
const glm::vec3 SLIME_EYE_SCALE{10, 15, 1};
const EnemyInfo ENEMY_FLY_INFO{0, 2, 30, 50, 600};

class Enemy : public Entity {
  public:
    inline float getHealth() const { return this->m_info.hp; }
    inline void setHealth(const float &health) { this->m_info.hp = health; }
    inline void heal(const float &heal) { this->m_info.hp += heal; }
    inline void damage(const float &damage) { this->m_info.hp -= damage; }
    inline bool isDead() const { return this->m_info.hp <= 0; }

    inline float getDamage() const { return this->m_info.damage; }

    inline float getLastShoot() const { return this->m_info.lastShoot; }
    inline void setLastShoot(const float &time) { this->m_info.lastShoot = time; }

    inline float getCooldown() const { return this->m_info.cooldown; }
    inline void setCooldown(const float &cooldown) { this->m_info.cooldown = cooldown; }

    inline float getRange() const { return this->m_info.range; }
    inline void setRange(const float &range) { this->m_info.range = range; }

    inline void setShootCallback(std::function<void()> &&func) { this->m_shootCallback = std::move(func); }

    inline void shoot(const float &time) {
        ASSERT(this->m_shootCallback != nullptr);

        // can shoot
        if (time - this->m_info.lastShoot > this->m_info.cooldown || this->m_info.lastShoot == 0) {
            this->m_shootCallback();
        }
    }

    Enemy() = default;
    Enemy(const EnemyInfo &info, const EntityVertex &vertex, const EntityModel &model);

    ~Enemy() = default;

  private:
    EnemyInfo m_info{};

    std::function<void()> m_shootCallback = nullptr;
};
