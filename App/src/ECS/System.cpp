#include "../../include/ECS/System.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <vector>
#include "../../include/ECS/Component.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/Ett.hpp"

const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

const auto BOUNDING_BOX_COLOR = glm::vec4{1, 0, 0, 1};

struct BoundingBox {
	ogl::ShaderProgram program = ogl::ShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
	ogl::VertexArray vao{};
	ogl::VertexBuffer vbog{};
	ogl::VertexBuffer vboc{};
	std::vector<glm::vec3> coords{};
	std::vector<glm::vec4> colors{};
	bool init = false;
} defaultShader;

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

		glm::vec3 getScale(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->scale;
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
			// std::cerr << "Optimize this method (read comment above implementation): LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
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
			if (ic == nullptr)
				return;

			ic->registerAction(key, func);
		}
	} // namespace input

	namespace gun {
		float getLastShoot(const unsigned int &id) {
			auto c = em->getComponentFromId<GunComponent>(id);
			ASSERT(c != nullptr);

			return c->info.lastShoot;
		}
		void updateLastShoot(const unsigned int &id, const float &time) {
			auto c = em->getComponentFromId<GunComponent>(id);
			ASSERT(c != nullptr);

			c->info.lastShoot = time;
		}

		float getCooldown(const unsigned int &id) {
			auto c = em->getComponentFromId<GunComponent>(id);
			ASSERT(c != nullptr);

			return c->info.coolDown;
		}
	} // namespace gun

	namespace animation {
		void executeNextFrame(const Shared<BasicScene> &scene, const float &currentTime) {
			std::vector<unsigned int> torm{};
			for (auto ett : em->getEntitiesFromComponent<Animation>()) {
				auto c = em->getComponentFromId<Animation>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(currentTime);

				if (c->dead) {
					torm.push_back(ett);
				}
			}
			for (auto e : torm) {
                EntityManager::instance()->removeEntity(e);
                scene->removeEntity(e);
			}
		}
	} // namespace animation

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

		void renderBoundingBox() {
			if (!defaultShader.init) {
				defaultShader.program.createShaderProgram();
				defaultShader.vao.onAttach();
				defaultShader.vbog.onAttach();
				defaultShader.vboc.onAttach();
				for (int i = 0; i < 8; i++)
					defaultShader.colors.push_back(BOUNDING_BOX_COLOR);
				defaultShader.init = true;
			}

			defaultShader.program.use();
			for (auto ett : em->getEntitiesFromComponent<AABB>()) {
				auto box = em->getComponentFromId<AABB>(ett);
				defaultShader.coords.clear();
				defaultShader.coords.push_back(box->botLeft);
				defaultShader.coords.push_back({box->topRight.x, box->botLeft.y, 0});
				defaultShader.coords.push_back({box->topRight.x, box->botLeft.y, 0});
				defaultShader.coords.push_back(box->topRight);
				defaultShader.coords.push_back(box->topRight);
				defaultShader.coords.push_back({box->botLeft.x, box->topRight.y, 0});
				defaultShader.coords.push_back({box->botLeft.x, box->topRight.y, 0});
				defaultShader.coords.push_back(box->botLeft);

				defaultShader.vao.bind();
				defaultShader.vbog.setup(defaultShader.coords.data(), defaultShader.coords.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				defaultShader.vboc.setup(defaultShader.colors.data(), defaultShader.colors.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

				defaultShader.program.setMat4("model", glm::mat4(1)[0][0]);
				glDrawArrays(GL_LINES, 0, defaultShader.coords.size());
			}
		}
	} // namespace render

} // namespace systems
