#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include "../include/AppGui.hpp"
#include "../include/Enemy.hpp"
#include "../include/Factory.hpp"
#include "../include/MultiShape.hpp"
#include "../include/Projectile.hpp"

#include "../include/ECS/Component.hpp"
#include "../include/ECS/Ett.hpp"
#include "../include/ECS/System.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "../include/ECS/EcsScene.hpp"
#include "../include/HermiteFactory.hpp"

Shared<Enemy> enem;

using namespace ogl;

const auto em = EventManager::instance();

const glm::vec3 PLAYER_VEL = {50, 50, 0};
const glm::vec3 PROJ_OFFSET = {2, 2, 0};
const glm::vec3 HEALTH_BAR_SIZE = {300, 20, 0};

bool outOfScreen(const Pair<float> &size, const glm::vec3 &pos, const glm::vec3 &scale) {
	return (pos.y + scale.y > size.y) || (pos.y - scale.y < 0) || (pos.x + scale.x > size.x) || (pos.x - scale.x < 0);
}

// void shootProjectile(const glm::vec3 &direction, const glm::vec3 &pos, const glm::vec3 &scale, Scene &scene, const ShaderProgram &shader) {
// 	if (direction == glm::vec3{0, 0, 0})
// 		return;
//
// 	if (glfwGetTime() - enem->getLastShoot() > enem->getCooldown() || enem->getCooldown() == 0) {
// 		// create projectile and add to the scene
// 		glm::vec3 offset{0, 0, 0};
// 		if (direction.x == 0) {
// 			// Y-Axis
// 			offset.y = direction.y * (scale.y / 2 + PROJ_SIZE.y / 2);
// 		} else {
// 			// X-Axis
// 			offset.x = direction.x * (scale.x / 2 + PROJ_SIZE.x / 2);
// 		}
// 		auto p = CreateShared<Projectile>(ProjInfo{enem->getDamage(), enem->getRange()}, pos + offset + (direction * PROJ_OFFSET), shader);
// 		p->setVelocity(direction * PROJ_VEL);
// 		p->init();
// 		scene.addEntity(shader.getId(), std::move(p));
// 		enem->setLastShoot(glfwGetTime());
// 	}
// }

int main(int argc, char *argv[]) {
	// WindowSettings s{};
	// s.decorated = false;
	//
	// Window w{s};
	// w.onAttach();
	// em->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	// em->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });
	//
	// ImGuiManager im{"", w};
	// im.onAttach();
	// em->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	// em->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	// em->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	// em->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });
	//
	// const auto rm = ResourceManager::instance();
	//
	// Scene basic{};
	// basic.onAttach();
	// em->subscribe(event::loop::LOOP_RENDER, [&basic]() { basic.onRender(); });
	//
	// auto igscene = im.addPanel<ImGuiScene>(basic);
	// igscene->setRenderFunc([&basic]() {
	// 	ImGui::Begin("Basic Scene");
	// 	auto b = basic.isDebugEnabled();
	// 	if (ImGui::Checkbox("View BB", &b)) {
	// 		basic.setDebug(b);
	// 	}
	// 	ImGui::End();
	// });
	//
	// ShaderProgram shader{"vertexShader.glsl", "fragmentShader.glsl"};
	// shader.createShaderProgram();
	//
	// auto back = createSquare({w.getWidth() / 2, w.getHeight() / 2, 0}, {w.getWidth(), w.getHeight(), 1}, {}, {1, 1, 0, 0.2});
	// back->init();
	// back->setRenderCall([&back, &shader]() {
	// 	shader.setMat4("model", back->getModelMatrix());
	// 	back->bindVAO();
	// 	glDrawElements(GL_TRIANGLES, back->getIndicesVector().size(), GL_UNSIGNED_INT, 0);
	// });
	// basic.addEntity(shader.getId(), back);
	//
	// auto bubu = createSquare({w.getWidth() / 2, w.getHeight() / 2, 0}, {50, 50, 1}, {}, {1, 1, 0, 1});
	// bubu->init();
	// bubu->setCollidable(true);
	// bubu->setRenderCall([&bubu, &shader]() {
	// 	shader.setMat4("model", bubu->getModelMatrix());
	// 	bubu->bindVAO();
	// 	glDrawElements(GL_TRIANGLES, bubu->getIndicesVector().size(), GL_UNSIGNED_INT, 0);
	// });
	// basic.addEntity(shader.getId(), bubu);
	//
	// // basic multishape entity
	// auto leye = createCircle({385, 710, 0}, {10, 13, 1}, {}, {0, 0}, {1, 1}, 30, {0, 0, 0, 1});
	// leye->init();
	// leye->setRenderCall([&leye, &shader]() {
	// 	shader.setMat4("model", leye->getModelMatrix());
	// 	leye->bindVAO();
	// 	glDrawArrays(GL_TRIANGLE_FAN, 0, leye->getCoordsVector().size());
	// });
	// auto reye = createCircle({415, 710, 0}, {10, 13, 1}, {}, {0, 0}, {1, 1}, 30, {0, 0, 0, 1});
	// reye->init();
	// reye->setRenderCall([&reye, &shader]() {
	// 	shader.setMat4("model", reye->getModelMatrix());
	// 	reye->bindVAO();
	// 	glDrawArrays(GL_TRIANGLE_FAN, 0, reye->getCoordsVector().size());
	// });
	// auto quad = createSquare({400, 700, 0}, {60, 60, 1}, {}, {1, 1, 0, 1});
	// quad->init();
	// quad->setRenderCall([&quad, &shader]() {
	// 	shader.setMat4("model", quad->getModelMatrix());
	// 	quad->bindVAO();
	// 	glDrawElements(GL_TRIANGLES, quad->getIndicesVector().size(), GL_UNSIGNED_INT, 0);
	// });
	//
	// Shared<MultiShape> sh = CreateShared<MultiShape>(shader, quad);
	// sh->addEntity(leye);
	// sh->addEntity(reye);
	// basic.addEntity(shader.getId(), sh);
	//
	// // hermite
	// auto mesh = readDataFromFile("./resources/hermite/other.txt");
	// buildHermite({1, 0, 0, 1}, {1, 0, 0, 1}, mesh);
	// auto ent = CreateShared<Entity>(EntityVertex{mesh->vertex, mesh->colors, {}}, EntityModel{});
	// ent->setPosition({400, 400, 0});
	// ent->setScale({40, 40, 0});
	// ent->setCollidable(true);
	// ent->init();
	// ent->setRenderCall([&shader, &ent]() {
	// 	shader.setMat4("model", ent->getModelMatrix());
	// 	ent->bindVAO();
	// 	glDrawArrays(GL_TRIANGLE_FAN, 0, ent->getCoordsVector().size());
	// });
	// basic.addEntity(shader.getId(), ent);
	//
	// {
	// 	auto var = createSquare({800, 50, 0}, {20, 40, 1}, {}, {1, 1, 1, 1});
	// 	enem = CreateShared<Enemy>(EnemyInfo{0, 0.6, 20, 100, 400}, EntityVertex{var->getCoordsVector(), var->getColorVector(), var->getIndicesVector()},
	// 	                           EntityModel{{800, 50, 0}, {20, 40, 1}, {}});
	// }
	// enem->init();
	// enem->setRenderCall([&shader]() {
	// 	shader.setMat4("model", enem->getModelMatrix());
	// 	enem->bindVAO();
	// 	glDrawElements(GL_TRIANGLES, enem->getIndicesVector().size(), GL_UNSIGNED_INT, 0);
	// });
	// enem->setShootCallback([]() {
	// });
	// auto healthBar = createSquare({10 + (HEALTH_BAR_SIZE.x / 2), w.getHeight() - 20, 0}, HEALTH_BAR_SIZE, {}, {1, 0, 0, 1});
	// healthBar->init();
	// healthBar->setRenderCall([healthBar, &shader]() {
	// 	shader.setMat4("model", healthBar->getModelMatrix());
	// 	healthBar->bindVAO();
	// 	glDrawElements(GL_TRIANGLES, healthBar->getIndicesVector().size(), GL_UNSIGNED_INT, 0);
	// });
	// basic.addEntity(shader.getId(), healthBar);
	//
	// basic.addEntity(shader.getId(), enem);
	//
	// em->subscribe(event::loop::LOOP_INPUT, [&w, &basic, &shader]() {
	// 	auto pos = enem->getPosition();
	// 	auto scale = enem->getScale();
	//
	// 	// movement
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_W) == GLFW_PRESS) {
	// 		pos += PLAYER_VEL * glm::vec3(0, 1, 0) * 0.1f;
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_S) == GLFW_PRESS) {
	// 		pos += PLAYER_VEL * glm::vec3(0, -1, 0) * 0.1f;
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_A) == GLFW_PRESS) {
	// 		pos += PLAYER_VEL * glm::vec3(-1, 0, 0) * 0.1f;
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_D) == GLFW_PRESS) {
	// 		pos += PLAYER_VEL * glm::vec3(1, 0, 0) * 0.1f;
	// 	}
	// 	enem->setPosition(pos);
	// 	// update the player pos
	//
	// 	// shoot
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_RIGHT)) {
	// 		shootProjectile({1, 0, 0}, pos, scale, basic, shader);
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_LEFT)) {
	// 		shootProjectile({-1, 0, 0}, pos, scale, basic, shader);
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_UP)) {
	// 		shootProjectile({0, 1, 0}, pos, scale, basic, shader);
	// 	}
	// 	if (glfwGetKey(w.getContext(), GLFW_KEY_DOWN)) {
	// 		shootProjectile({0, -1, 0}, pos, scale, basic, shader);
	// 	}
	// });
	//
	// // collision solver
	// em->subscribe(event::loop::LOOP_UPDATE, [&w, &basic]() {
	// 	// player solver
	// 	{
	// 		auto pos = enem->getPosition();
	// 		auto scale = enem->getScale();
	// 		if (pos.y + scale.y > w.getHeight()) {
	// 			pos = {pos.x, w.getHeight() - scale.y, 0};
	// 		}
	// 		if (pos.y - scale.y < 0) {
	// 			pos = {pos.x, scale.y, 0};
	// 		}
	// 		if (pos.x - scale.x < 0) {
	// 			pos = {scale.x, pos.y, 0};
	// 		}
	// 		if (pos.x + scale.x > w.getWidth()) {
	// 			pos = {w.getWidth() - scale.x, pos.y, 0};
	// 		}
	// 		enem->setPosition(pos);
	// 	}
	// 	// bullets collision
	// 	std::vector<Shared<Entity>> toRemove{};
	// 	{
	// 		for (auto e : basic.getAllEntities()) {
	// 			// the entity is a projectile
	// 			if (std::dynamic_pointer_cast<Projectile>(e) != nullptr) {
	// 				if (outOfScreen({w.getWidth(), w.getHeight()}, e->getPosition(), e->getScale())) {
	// 					basic.removeEntity(e);
	// 					continue;
	// 				}
	// 				auto cast = std::static_pointer_cast<Projectile>(e);
	//
	// 				// bullet collision
	// 				auto bullet = cast->getBoundingBox();
	// 				for (auto ent : basic.getAllEntities()) {
	// 					// no collision between projectiles
	// 					if (std::dynamic_pointer_cast<Projectile>(ent) == nullptr) {
	// 						if (ent->isCollidable()) {
	// 							auto bb = ent->getBoundingBox();
	// 							bool cx = bb.botLeft.x <= bullet.topRight.x && bb.topRight.x >= bullet.botLeft.x;
	// 							bool cy = bb.botLeft.y <= bullet.topRight.y && bb.topRight.y >= bullet.botLeft.y;
	// 							if (cx && cy) {
	// 								auto ce = std::dynamic_pointer_cast<Enemy>(ent);
	// 								if (ce != nullptr) {
	// 									ce->damage(cast->getDamage());
	// 									if (ce->isDead()) {
	// 										if (std::find(ALL(toRemove), ce) == toRemove.end())
	// 											toRemove.push_back(ent);
	// 									}
	// 									cast->endLife();
	// 								}
	// 							}
	// 						}
	// 					}
	// 				}
	//
	// 				if (cast->isDead()) {
	// 					if (std::find(ALL(toRemove), cast) == toRemove.end())
	// 						toRemove.push_back(cast);
	// 					continue;
	// 				}
	// 			}
	// 		}
	// 	}
	// 	for (auto e : toRemove) {
	// 		basic.removeEntity(e);
	// 	}
	// });
	//
	// UniformBuffer ubo{"Matrices"};
	// glm::mat4 proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
	// ubo.onAttach();
	// ubo.setup(sizeof(glm::mat4), 0, 0, 0);
	// ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	// em->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&w, &ubo]() {
	// 	auto proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
	// 	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	// });
	//
	// auto gg = createEnemy(shader, EnemyType::EMENY_SLIME, {{800, 800, 1}, ENEMY_SLIME_SIZE, {}});
	// gg->setCollidable(true);
	// basic.addEntity(shader.getId(), gg);
	//
	// em->subscribe(event::loop::LOOP_UPDATE,
	//               [&gg]() { gg->moveEntities(gg->getPosition() + glm::vec3(1, 0, 0) * glm::vec3(10) * glm::vec3(glm::sin(glfwGetTime() * 4))); });
	//
	// while (!glfwWindowShouldClose(w.getContext())) {
	// 	em->post(event::loop::LOOP_INPUT);
	// 	em->post(event::loop::LOOP_UPDATE);
	// 	em->post(event::loop::LOOP_BEGIN_RENDER);
	// 	em->post(event::loop::LOOP_RENDER);
	// 	em->post(event::loop::LOOP_END_RENDER);
	// }
	//
	// basic.onDetach();
	// im.onDetach();
	// w.onDetach();

	WindowSettings s{};
	s.decorated = false;
	Window w{s};
	w.onAttach();
	em->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	em->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	ImGuiManager im{"", w};
	im.onAttach();
	em->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	em->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	em->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	em->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	im.addPanel<ImGuiStats>();

	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();

	const auto ecs = BasicScene::instance();

	auto ett = EntityManager::instance();

	auto first = factorySquare(BasicInfo{{1400, 800, 0}, {40, 40, 1}, {}}, {1, 0, 0, 1});
	ecs->addEntity(shader, first);

	systems::input::setKeyCallback(first, GLFW_KEY_W, [&first]() {
		systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(0, 1, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_A, [&first]() {
		systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(-1, 0, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_S, [&first]() {
		systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(0, -1, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_D, [&first]() {
		systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(1, 0, 0));
	});

	auto second = factoryProjectile(BasicInfo{{1000, 800, 0}, {40, 40, 1}, {}}, {1, 1, 0, 1}, {});
	ecs->addEntity(shader, second);

	auto third = factoryHermite(BasicInfo{{1050, 800, 0}, ENEMY_SLIME_SIZE, {}}, "./resources/hermite/slime.txt");
	ecs->addEntity(shader, third);

	// Entity Manager callbacks
	std::cerr << "Move this operation in EventManger: LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
	em->subscribe(event::loop::LOOP_INPUT, [&w, &ett]() {
		for (auto e : ett->getEntitiesFromComponent<InputComponent>()) {
			for (auto [key, f] : systems::input::getKeysCallback(e)) {
				if (glfwGetKey(w.getContext(), key) == GLFW_PRESS) {
					f();
				}
			}
		}
	});

	UniformBuffer ubo{"Matrices"};
	glm::mat4 proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
	ubo.onAttach();
	ubo.setup(sizeof(glm::mat4), 0, 0, 0);
	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	em->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&w, &ubo]() {
		auto proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
		ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	});

	auto igdebug = im.addPanel<ImGuiDebug>();
	igdebug->setRenderFunc([&igdebug]() {
		ImGui::Begin("Debug");
		auto b = igdebug->isBoundingBoxVisible();
		if (ImGui::Checkbox("View BB", &b)) {
			igdebug->showBoundingBox(b);
		}
        ImGui::Text("Collision count: %ld", systems::collision::getCollisions().size());
		ImGui::End();
	});

	auto igscene = im.addPanel<ImGuiModel>(first);
	igscene->setRenderFunc([&ett, &igscene]() {
		auto tc = ett->getComponentFromId<Transform>(igscene->getCurrentId());
		auto bc = ett->getComponentFromId<AABB>(igscene->getCurrentId());
		if (tc == nullptr)
			return;
		if (bc == nullptr)
			return;

		ImGui::Begin("Basic Scene");
		auto pos = systems::transform::getPosition(igscene->getCurrentId());
		if (ImGui::DragFloat3("Pos", &pos[0])) {
			systems::transform::updatePosition(igscene->getCurrentId(), pos);
		}
		ImGui::End();
	});

	// collision manager
	// em->subscribe(event::loop::LOOP_UPDATE, [&first, &second]() {
	// auto coll = systems::collision::getCollisions();
	// std::cout << coll.size() << "\n";
	// });
	em->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });
	em->subscribe(event::loop::LOOP_RENDER, [&igdebug]() { if (igdebug->isBoundingBoxVisible()) systems::render::renderBoundingBox(); });

	while (!glfwWindowShouldClose(w.getContext())) {
		em->post(event::loop::LOOP_INPUT);
		em->post(event::loop::LOOP_UPDATE);
		em->post(event::loop::LOOP_BEGIN_RENDER);
		em->post(event::loop::LOOP_RENDER);
		em->post(event::loop::LOOP_END_RENDER);
	}

	im.onDetach();
	w.onDetach();

	return 0;
}
