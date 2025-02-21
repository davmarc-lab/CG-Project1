#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"
#include "EcsScene.hpp"
#include "Ett.hpp"

namespace systems {
	namespace ecs {
		bool removeEntityFromManager(const unsigned int &id);
		void removeEntityFromScene(const unsigned int &id);
		bool removeEntityFromAll(const unsigned int &id);
	} // namespace ecs

	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position);
		void updateScale(const unsigned int &id, const glm::vec3 &scale);
		void updateRotation(const unsigned int &id, const glm::vec3 &rotation);

		void addPosition(const unsigned int &id, const glm::vec3 &offset);
		void addScale(const unsigned int &id, const glm::vec3 &offset);
		void addRotation(const unsigned int &id, const glm::vec3 &offset);

		glm::vec3 getPosition(const unsigned int &id);
		glm::vec3 getScale(const unsigned int &id);
		glm::vec3 getRotation(const unsigned int &id);
		glm::mat4 getModelMatrix(const unsigned int &id);

		void updateModelMatrix(const unsigned int &id);

		void updateAllModelMatrix();

	} // namespace transform

	namespace collision {
		void updateCollider(const unsigned int &id);
		void updateAllColliders();

		void resolveCollisions();

		float getEnemyLastHit(const unsigned int &id);
		float getPlayerLastHit(const unsigned int &id);

		void updateEnemyLastHit(const unsigned int &id, const float &time);
		void updatePlayerLastHit(const unsigned int &id, const float &time);

		std::vector<Pair<unsigned int>> getCollisions();
	} // namespace collision

	namespace input {
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id);

		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func);
	} // namespace input

	namespace gun {
		float getLastShoot(const unsigned int &id);
		void updateLastShoot(const unsigned int &id, const float &time);

		float getCooldown(const unsigned int &id);
	} // namespace gun

	namespace animation {
		void executeNextFrame(const float &currentTime);
		void updateDistanceAnimation();
	} // namespace animation

	namespace render {
		void renderAllMeshes();

		void renderBoundingBox();
	} // namespace render
} // namespace systems
