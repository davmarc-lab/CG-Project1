#include "../../include/ECS/System.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <set>
#include <vector>
#include "../../include/ECS/Component.hpp"
#include "../../include/ECS/EcsScene.hpp"
#include "../../include/ECS/Ett.hpp"

#include "../../include/LevelManager.hpp"

const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

const auto BOUNDING_BOX_COLOR = glm::vec4{1, 0, 0, 1};

const auto HIT_PROJ_COOLDOWN = .6f;
const auto HIT_ENEMY_COOLDOWN = 1.f;

struct BoundingBox {
	ogl::ShaderProgram program = ogl::ShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
	ogl::VertexArray vao{};
	ogl::VertexBuffer vbog{};
	ogl::VertexBuffer vboc{};
	std::vector<glm::vec3> coords{};
	std::vector<glm::vec4> colors{};
	bool init = false;
} defaultShader;

ogl::ShaderProgram stencil = ogl::ShaderProgram("singlevs.glsl", "stencilShader.glsl");

namespace systems {
	namespace ecs {
		bool removeEntityFromManager(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					removeEntityFromManager(child);
				}
			}
			return em->removeEntity(id);
		}
		void removeEntityFromScene(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					scene->removeEntity(child);
				}
			}
			scene->removeEntity(id);
		}
		bool removeEntityFromAll(const unsigned int &id) {
			removeEntityFromScene(id);
			return removeEntityFromManager(id);
		}
	} // namespace ecs
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto offset = cpos - c->getPosition();
					auto res = position + offset;
					res.z = cpos.z;
					updatePosition(child, res);
				}
			}

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

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					updateRotation(child, rotation);
				}
			}

			c->setRotation(rotation);
		}

		void addPosition(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto modelOffset = cpos - c->getPosition();
					auto res = c->getPosition() + modelOffset + offset;
					res.z = cpos.z;
					updatePosition(child, res);
				}
			}

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
			// ASSERT(tc != nullptr);

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

	namespace enemy {
		float getHealth(const unsigned int &id) {
			auto c = em->getComponentFromId<HealthComponent>(id);
			ASSERT(c != nullptr);

			return c->health;
		}

		float getDamage(const unsigned int &id) {
			auto c = em->getComponentFromId<EnemyComponent>(id);
			ASSERT(c != nullptr);

			return c->damage;
		}

		void decreaseHealth(const unsigned int &id, const float &health) {
			auto c = em->getComponentFromId<HealthComponent>(id);
			ASSERT(c != nullptr);

			c->health -= health;
		}
		void incrementHealth(const unsigned int &id, const float &health) {
			auto c = em->getComponentFromId<HealthComponent>(id);
			ASSERT(c != nullptr);

			c->health += health;
		}

		void setBehaviour(const unsigned int &id, std::function<void()> &&func) {
			auto c = em->getComponentFromId<BehaviourComponent>(id);
			ASSERT(c != nullptr);

			c->func = std::move(func);
		}

		void execBehaviourFunc(const unsigned int &id) {
			auto c = em->getComponentFromId<BehaviourComponent>(id);
			ASSERT(c != nullptr);

			if (c->func != nullptr)
				c->func();
		}
		void execAllBehaviourFunc() {
			for (auto id : em->getEntitiesFromComponent<BehaviourComponent>()) {
				execBehaviourFunc(id);
			}
		}
	} // namespace enemy

	namespace collision {
		void updateParentCollider(const unsigned int &id) {
			auto box = em->getComponentFromId<AABB>(id);
			auto tc = em->getComponentFromId<Transform>(id);
			auto offset = tc->position - box->position;
			box->position = tc->position;
			box->botLeft += offset;
			box->topRight += offset;
		}

		void updateCollider(const unsigned int &id) {
			auto bc = em->getComponentFromId<AABB>(id);
			if (bc == nullptr)
				return;

			auto pc = em->getComponentFromId<ParentComponent>(id);
			// has children
			if (pc != nullptr && !pc->children.empty()) {
				updateParentCollider(id);
				return;
			}

			// default collider update
			auto cc = em->getComponentFromId<VertexComponent>(id);
			bc->updateCollider(cc->getVertexCoords(), ::systems::transform::getModelMatrix(id));
		}

		void updateAllColliders() {
			auto ids = em->getEntitiesFromComponent<AABB>();
			for (auto id : em->getEntitiesFromComponent<AABB>()) {
				updateCollider(id);
			}
		}

		bool isColliding(const unsigned int &first, const unsigned int &second) {
			if (first == second)
				return false;
			auto fc = em->getComponentFromId<AABB>(first);
			auto sc = em->getComponentFromId<AABB>(second);

			return fc->isColliding(*sc);
		}

		float getEnemyLastHit(const unsigned int &id) {
			auto c = em->getComponentFromId<EnemyComponent>(id);
			ASSERT(c != nullptr);

			return c->lastHit;
		}

		float getPlayerLastHit(const unsigned int &id) {
			auto c = em->getComponentFromId<PlayerComponent>(id);
			ASSERT(c != nullptr);

			return c->lastHit;
		}

		void updateEnemyLastHit(const unsigned int &id, const float &time) {
			auto c = em->getComponentFromId<EnemyComponent>(id);
			ASSERT(c != nullptr);

			c->lastHit = time;
		}

		void updatePlayerLastHit(const unsigned int &id, const float &time) {
			auto c = em->getComponentFromId<PlayerComponent>(id);
			ASSERT(c != nullptr);

			c->lastHit = time;
		}

		Pair<glm::vec3> getCollider(const unsigned int &id) {
			auto c = em->getComponentFromId<AABB>(id);
			ASSERT(c != nullptr);

			return {c->botLeft, c->topRight};
		}

		void compressBoundingBox() {
			for (auto id : em->getEntitiesFromComponent<ParentComponent>()) {
				auto c = em->getComponentFromId<ParentComponent>(id);
				auto pv = em->getComponentFromId<VertexComponent>(id);
				auto box = em->getComponentFromId<AABB>(id);
				// remove AABB component from each children and update the parent
				for (auto child : c->children) {
					if (em->entityHasComponent<AABB>(child)) {
						auto cv = em->getComponentFromId<VertexComponent>(child);
						auto model = ::systems::transform::getModelMatrix(child);
						auto bot = glm::vec3(1, 1, 0);
						bool first = true;

						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								bot = elem;
								first = false;
							}
							bot.x = bot.x >= elem.x ? elem.x : bot.x;
							bot.y = bot.y >= elem.y ? elem.y : bot.y;
						}

						auto top = glm::vec3(1, 1, 0);
						first = true;
						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								top = elem;
								first = false;
							}
							top.x = top.x <= elem.x ? elem.x : top.x;
							top.y = top.y <= elem.y ? elem.y : top.y;
						}
						box->botLeft.x = box->botLeft.x < bot.x ? box->botLeft.x : bot.x;
						box->botLeft.y = box->botLeft.y < bot.y ? box->botLeft.y : bot.y;
						box->topRight.x = box->topRight.x > top.x ? box->topRight.x : top.x;
						box->topRight.y = box->topRight.y > top.y ? box->topRight.y : top.y;
						em->removeComponent<AABB>(child);
					}
				}
			}
		}

		void resolveCollisions() {
			auto time = glfwGetTime();
			auto rmv = std::set<unsigned int>{};
			auto colls = getCollisions();
			auto projs = em->getEntitiesFromComponent<ProjectileComponent>();
			auto enems = em->getEntitiesFromComponent<EnemyComponent>();
			auto plays = em->getEntitiesFromComponent<PlayerComponent>();
			for (auto c : colls) {
				if (c.x == c.y)
					continue;

				// proj - proj
				{
					// is projectile
					auto first = std::find(ALL(projs), c.x) != projs.end();
					auto second = std::find(ALL(projs), c.y) != projs.end();
					if (first && second) {
						continue;
					}
				}
				// enemy - enemy
				{
					auto first = std::find(ALL(enems), c.x) != enems.end();
					auto second = std::find(ALL(enems), c.y) != enems.end();
					if (first && second)
						continue;
				}
				// proj - enemy
				{
					auto first = std::find(ALL(projs), c.x) != projs.end();
					auto second = std::find(ALL(enems), c.y) != enems.end();
					if (first && second) {
						auto hit = getEnemyLastHit(c.y);
						if (hit + HIT_PROJ_COOLDOWN < time || hit == 0) {
							::systems::enemy::decreaseHealth(c.y, ::systems::gun::getDamage(c.x));
							updateEnemyLastHit(c.y, time);
							if (::systems::enemy::getHealth(c.y) <= 0) {
								if (rmv.size() < 1) {
									rmv.insert(c.y);
									ogl::EventManager::instance()->post(EVENT_ENEMY_DEAD);
								}
							}
						}
						rmv.insert(c.x);
						continue;
					}
				}
				// enemy - proj
				{
					auto first = std::find(ALL(enems), c.x) != enems.end();
					auto second = std::find(ALL(projs), c.y) != projs.end();
					if (first && second) {
						auto hit = getEnemyLastHit(c.x);
						if (hit + HIT_PROJ_COOLDOWN < time || hit == 0) {
							::systems::enemy::decreaseHealth(c.x, ::systems::gun::getDamage(c.y));
							updateEnemyLastHit(c.x, time);
							if (::systems::enemy::getHealth(c.x) <= 0) {
								if (rmv.size() < 1) {
									rmv.insert(c.x);
									ogl::EventManager::instance()->post(EVENT_ENEMY_DEAD);
								}
							}
						}
						rmv.insert(c.y);
						continue;
					}
				}
				// player - enemy
				{
					auto first = std::find(ALL(plays), c.x) != plays.end();
					auto second = std::find(ALL(enems), c.y) != enems.end();
					if (first && second) {
						auto hit = getPlayerLastHit(c.x);
						if (hit + HIT_ENEMY_COOLDOWN < time || hit == 0) {
							::systems::enemy::decreaseHealth(c.x, ::systems::enemy::getDamage(c.y));
							updatePlayerLastHit(c.x, time);
							if (::systems::enemy::getHealth(c.x) <= 0) {
								ogl::EventManager::instance()->post(PLAYER_DEAD_EVENT);
							}
						}
						continue;
					}
				}
				// enemy - player
				{
					auto first = std::find(ALL(enems), c.x) != enems.end();
					auto second = std::find(ALL(plays), c.y) != plays.end();
					if (first && second) {
						auto hit = getPlayerLastHit(c.y);
						if (hit + HIT_ENEMY_COOLDOWN < time || hit == 0) {
							::systems::enemy::decreaseHealth(c.y, ::systems::enemy::getDamage(c.x));
							updatePlayerLastHit(c.y, time);
							if (::systems::enemy::getHealth(c.y) <= 0) {
								rmv.insert(c.y);
							}
						}
						continue;
					}
				}
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}

		std::vector<Pair<unsigned int>> getCollisions() {
			std::vector<Pair<unsigned int>> coll{};
			for (auto first : em->getEntitiesFromComponent<AABB>()) {
				for (auto other : em->getEntitiesFromComponent<AABB>()) {
					if (first == other)
						continue;
					{
						auto fc = em->getComponentFromId<ParentComponent>(first);
						if (fc != nullptr) {
							if (std::find(ALL(fc->children), other) != fc->children.end())
								continue;
						}
						auto oc = em->getComponentFromId<ParentComponent>(other);
						if (oc != nullptr) {
							if (std::find(ALL(oc->children), first) != oc->children.end())
								continue;
						}
					}
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

	namespace parent {
		void addChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.push_back(child);
		}

		void removeChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.erase(std::find(ALL(c->children), child));
		}
	} // namespace parent

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

		float getDamage(const unsigned int &id) {
			auto c = em->getComponentFromId<ProjectileComponent>(id);
			ASSERT(c != nullptr);

			return c->damage;
		}
	} // namespace gun

	namespace animation {
		void executeNextFrame(const float &currentTime) {
			std::vector<unsigned int> rmv{};
			for (auto ett : em->getEntitiesFromComponent<TimeAnimation>()) {
				auto c = em->getComponentFromId<TimeAnimation>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(currentTime);
				if (c->dead)
					rmv.push_back(ett);
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}

		void updateDistanceAnimation() {
			std::vector<unsigned int> rmv{};
			for (auto ett : em->getEntitiesFromComponent<ProjectileComponent>()) {
				auto c = em->getComponentFromId<ProjectileComponent>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(systems::transform::getPosition(ett));
				if (c->dead)
					rmv.push_back(ett);
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}
	} // namespace animation

	namespace render {
		void initStencilShader() {
			stencil.createShaderProgram();
		}

		void renderAllMeshes() {
			auto outlines = em->getEntitiesFromComponent<Outlined>();
			auto shader = scene->getShader();
			ASSERT(shader != nullptr);
			for (auto [stype, etts] : scene->getShaderEntityMap()) {
				shader->use();
				shader->setInt("shaderProgram", stype);
				for (auto id : etts) {
					if (std::find(ALL(outlines), id) == outlines.end()) {
						glStencilMask(0x00);
						glStencilFunc(GL_ALWAYS, 0, 0xFF);
						auto rc = em->getComponentFromId<RenderComponent>(id);
						shader->setMat4("model", ::systems::transform::getModelMatrix(id));
						rc->call();
					} else {
						// render normal mesh
						glStencilFunc(GL_ALWAYS, 1, 0xFF);
						glStencilMask(0xFF);
						auto rc = em->getComponentFromId<RenderComponent>(id);
						shader->setMat4("model", ::systems::transform::getModelMatrix(id));
						rc->call();
					}
				}
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				stencil.use();
				stencil.setInt("shaderProgram", ShaderType::SHADER_DEFAULT);
				for (auto id : outlines) {
					if (std::find(ALL(outlines), id) != outlines.end()) {
						// render a bigger mesh using stencil buffer
						auto model = glm::mat4(1);
						model = glm::translate(model, ::systems::transform::getPosition(id));
						model = glm::scale(model, ::systems::transform::getScale(id) + glm::vec3{4});
						// miss rotation
						stencil.setMat4("model", model);
						auto rc = em->getComponentFromId<RenderComponent>(id);
						rc->call();
					}
				}
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);
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

				defaultShader.program.setMat4("model", glm::mat4(1));
				glDrawArrays(GL_LINES, 0, defaultShader.coords.size());
			}
		}
	} // namespace render

} // namespace systems
