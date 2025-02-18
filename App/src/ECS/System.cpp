#include "../../include/ECS/System.hpp"
#include "../../include/ECS/Component.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/Ett.hpp"

const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

namespace systems {
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<Transform>(id);
			c->setPosition(position);
		}

		void updateScale(const unsigned int &id, const glm::vec3 &scale) {
			auto c = em->getComponentFromId<Transform>(id);
			c->setScale(scale);
		}

		void updateRotation(const unsigned int &id, const glm::vec3 &rotation) {
			auto c = em->getComponentFromId<Transform>(id);
			c->setRotation(rotation);
		}

		void addPosition(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			c->addPosition(offset);
		}

		void addScale(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			c->addScale(offset);
		}

		void addRotation(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			c->addRotation(offset);
		}
	} // namespace transform

	namespace render {
		void renderAllMeshes() {

			for (auto [shader, etts] : ecs->getShaderEntityMap()) {
				shader->use();
				for (auto id : etts) {
					auto vc = em->getComponentFromId<Transform>(id);
					auto rc = em->getComponentFromId<RenderComponent>(id);
					shader->setMat4("model", vc->getModelMatrix());
					rc->call();
				}
			}

			for (auto id : EntityManager::instance()->getEntitiesFromComponent<RenderComponent>()) {
				auto rend = EntityManager::instance()->getComponentFromId<RenderComponent>(id);
				rend->call();
			}
		}

	}

} // namespace systems
