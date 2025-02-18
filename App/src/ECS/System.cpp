#include "../../include/ECS/System.hpp"
#include <algorithm>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include "../../include/ECS/Component.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/Ett.hpp"

const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

namespace systems {
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->setPosition(position);
		}

		void updateScale(const unsigned int &id, const glm::vec3 &scale) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->setScale(scale);
		}

		void updateRotation(const unsigned int &id, const glm::vec3 &rotation) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->setRotation(rotation);
		}

		void addPosition(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addPosition(offset);
		}

		void addScale(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addScale(offset);
		}

		void addRotation(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addRotation(offset);
		}

		glm::vec3 getPosition(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->position;
		}

		glm::mat4 getModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			ASSERT(tc != nullptr);

			if (tc->isDirty())
				updateModelMatrix(id);
			return tc->model;
		}

		void updateModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			if (tc == nullptr)
				return;

			tc->updateModelMatrix();
			::systems::collision::updateCollider(id);
		}

		void updateAllModelMatrix() {
			for (auto id : em->getEntitiesFromComponent<Transform>()) {
				updateModelMatrix(id);
			}
		}
	} // namespace transform

	namespace collision {
		void updateCollider(const unsigned int &id) {
			auto bc = em->getComponentFromId<AABB>(id);
			if (bc == nullptr)
				return;

			auto cc = em->getComponentFromId<VertexComponent>(id);
			bc->updateCollider(cc->getVertexCoords(), ::systems::transform::getModelMatrix(id));
		}

		// void updateAllColliders() {
		// 	for (auto id : em->getEntitiesFromComponent<AABB>())
		// 		updateCollider(id);
		// }

		bool isColliding(const unsigned int &first, const unsigned int &second) {
			if (first == second)
				return false;
			auto fc = em->getComponentFromId<AABB>(first);
			auto sc = em->getComponentFromId<AABB>(second);

			return fc->isColliding(*sc);
		}

		std::vector<Pair<unsigned int>> getCollisions() {
			std::vector<Pair<unsigned int>> coll{};
			for (auto first : em->getEntitiesFromComponent<AABB>()) {
				for (auto other : em->getEntitiesFromComponent<AABB>()) {
					if (first == other)
						continue;
					if (isColliding(first, other)) {
						if (std::find(ALL(coll), Pair<unsigned int>{first, other}) == coll.end() &&
							std::find(ALL(coll), Pair<unsigned int>{other, first}) == coll.end()) {
							coll.push_back({first, other});
						}
					}
				}
			}
			return coll;
		}

	} // namespace collision

	namespace input {
		// it can be optimized merging from each InputComponent all callbacks.
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id) {
	        std::cerr << "Optimize this method (read comment above implementation): LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
			std::vector<std::pair<unsigned int, std::function<void()>>> res{};
			auto ic = em->getComponentFromId<InputComponent>(id);
            ASSERT(ic != nullptr);
            
			for (auto k : ic->getAllKeys()) {
				res.emplace_back(k, ic->callbacks.at(k));
			}
			return res;
		}

		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func) {
            auto ic = em->getComponentFromId<InputComponent>(id);
            if (ic == nullptr) return;

            ic->registerAction(key, func);
		}
	} // namespace input

	namespace render {
		void renderAllMeshes() {
			for (auto [shader, etts] : ecs->getShaderEntityMap()) {
				shader->use();
				for (auto id : etts) {
					auto rc = em->getComponentFromId<RenderComponent>(id);
					shader->setMat4("model", ::systems::transform::getModelMatrix(id));
					rc->call();
				}
			}
		}
	} // namespace render

} // namespace systems
