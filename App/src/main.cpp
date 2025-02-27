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
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "../include/ECS/EcsScene.hpp"
#include "../include/HermiteFactory.hpp"

Shared<Enemy> enem;

using namespace ogl;

const auto ed = EventManager::instance();

const glm::vec3 PLAYER_VEL = {50, 50, 0};
const glm::vec3 PROJ_OFFSET = {2, 2, 0};
const glm::vec3 HEALTH_BAR_SIZE = {200, 10, 0};
const float HP_FACTOR = 2.f;

struct Player {
	unsigned int id;
	GunInfo gunInfo{};
	ProjInfo projInfo{20, PROJ_RANGE, PROJ_COLOR};
} player;

bool outOfScreen(const Pair<float> &size, const glm::vec3 &pos, const glm::vec3 &scale) {
	return (pos.y + scale.y > size.y) || (pos.y - scale.y < 0) || (pos.x + scale.x > size.x) || (pos.x - scale.x < 0);
}

void playerShoot(unsigned int &id, const glm::vec3 &direction, const Shared<BasicScene> &scene, Shared<ogl::ShaderProgram> shader) {
	if (direction == glm::vec3(0))
		return;

	if (!EntityManager::instance()->entityHasComponent<GunComponent>(id))
		return;

	auto ls = systems::gun::getLastShoot(id);
	auto cd = systems::gun::getCooldown(id);

	if (glfwGetTime() - ls > cd || cd == 0) {
		auto box = systems::collision::getCollider(id);
		glm::vec3 pos{0};
		if (direction.x == 0) {
			pos.y = (box.x.y + (box.y.y - box.x.y) / 2) + direction.y * ((box.y.y - box.x.y) / 2);
			pos.x = (box.y.x - box.x.x) / 2 + box.x.x;
		} else {
			pos.x = (box.x.x + (box.y.x - box.x.x) / 2) + direction.x * ((box.y.x - box.x.x) / 2);
			pos.y = (box.y.y - box.x.y) / 2 + box.x.y;
		}
		auto offset = direction * (PROJ_SIZE * glm::vec3(1, 1, 0) + PROJ_OFFSET);
		scene->addEntity(shader, factoryProjectile(BasicInfo{{pos + offset}, {PROJ_SIZE}}, player.projInfo.color, player.projInfo, direction));
		systems::gun::updateLastShoot(id, glfwGetTime());
	}
}

int main(int argc, char *argv[]) {
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

	WindowSettings s{};
	s.decorated = false;
	s.vsync = true;
	Window w{s};
	w.onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	ImGuiManager im{"", w};
	im.onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	im.addPanel<ImGuiStats>();

	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();

	const auto ecs = BasicScene::instance();

	auto ett = EntityManager::instance();

	auto first = factoryHermite(BasicInfo{{1400, 800, 0}, {40, 50, 1}, {}}, "./resources/hermite/player/down.txt", {1, 0.7568, 0.9450, 1});
	ecs->addEntity(shader, first);
	ett->addComponent<GunComponent>(first, player.gunInfo);
	ett->addComponent<PlayerComponent>(first);
	ett->addComponent<HealthComponent>(first);
	ett->addComponent<ParentComponent>(first);

	auto head = factoryCircle(BasicInfo{{1400, 820, 1}, {20, 15, 1}, {}}, {1, 1, 0, 1}, 40);
	ett->addComponent<AABB>(head);
	systems::parent::addChild(first, head);
	ecs->addEntity(shader, head);

	auto healthBar = factorySquare(BasicInfo{{10 + HEALTH_BAR_SIZE.x, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 1}, HEALTH_BAR_SIZE, {}}, {1, 0, 0, 1});
	ecs->addEntity(shader, healthBar);

	ed->subscribe(event::loop::LOOP_UPDATE, [&healthBar, &first, &w]() {
		auto health = systems::enemy::getHealth(first);
		systems::transform::updatePosition(healthBar, {10 + health * HP_FACTOR, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 0});
		systems::transform::updateScale(healthBar, {health * HP_FACTOR, HEALTH_BAR_SIZE.y, 1});
	});

	// movement
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

	// shoot
	systems::input::setKeyCallback(first, GLFW_KEY_UP, [&first, ecs, shader]() {
		playerShoot(first, glm::vec3{0, 1, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_LEFT, [&first, ecs, shader]() {
		playerShoot(first, glm::vec3{-1, 0, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_DOWN, [&first, ecs, shader]() {
		playerShoot(first, glm::vec3{0, -1, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_RIGHT, [&first, ecs, shader]() {
		playerShoot(first, glm::vec3{1, 0, 0}, ecs, shader);
	});

	ed->subscribe(event::loop::LOOP_UPDATE, [&first, &w]() {
		auto pos = systems::transform::getPosition(first);
		auto scale = systems::transform::getScale(first);
		if (pos.y + scale.y > w.getHeight()) {
			pos = {pos.x, w.getHeight() - scale.y, 0};
		}
		if (pos.y - scale.y < 0) {
			pos = {pos.x, scale.y, 0};
		}
		if (pos.x - scale.x < 0) {
			pos = {scale.x, pos.y, 0};
		}
		if (pos.x + scale.x > w.getWidth()) {
			pos = {w.getWidth() - scale.x, pos.y, 0};
		}
		systems::transform::updatePosition(first, pos);
	});

	auto third = factoryEnemy(BasicInfo{{1050, 800, 0}, ENEMY_SLIME_SIZE, {}}, {1, 0, 0, 1});
	ecs->addEntity(shader, third);

	// Entity Manager callbacks
	std::cerr << "Move this operation in EventManger: LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
	ed->subscribe(event::loop::LOOP_INPUT, [&w, &ett]() {
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
	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&w, &ubo]() {
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
		ImGui::Text("Collision count: %zu", systems::collision::getCollisions().size());
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

		auto scale = systems::transform::getScale(igscene->getCurrentId());
		if (ImGui::DragFloat3("Scale", &scale[0])) {
			systems::transform::updateScale(igscene->getCurrentId(), scale);
		}
		ImGui::End();
	});

	// auto pad = im.addPanel<ImGuiPanel>();
	// pad->setRenderFunc([&w]() {
	// 	auto padid = GLFW_JOYSTICK_1;
	// 	ImGui::Begin("Pad");
	// 	ImGui::Text("Pad Connected: %d", glfwJoystickPresent(padid) == GLFW_TRUE);
	// 	int axisCount;
	// 	auto axis = glfwGetJoystickAxes(padid, &axisCount);
	// 	ImGui::Text("Axis Count: %d", axisCount);
	// 	for (int i = 0; i < axisCount; i++) {
	// 		ImGui::Text("Axis %d -> %f", i, axis[i]);
	// 	}
	// 	int buttonCount;
	// 	auto buttons = glfwGetJoystickButtons(padid, &buttonCount);
	// 	ImGui::Text("Button Count: %d", buttonCount);
	// 	for (int i = 0; i < buttonCount; i++) {
	// 		ImGui::Text("Button %d -> %d", i, buttons[i] == GLFW_PRESS);
	// 	}
	// 	int hatCount;
	// 	auto hats = glfwGetJoystickHats(padid, &hatCount);
	// 	ImGui::Text("Hats Count: %d", hatCount);
	// 	for (int i = 0; i < hatCount; i++) {
	// 		ImGui::Text("Hat %d -> %d", i, hats[i]);
	// 	}
	//
	// 	ImGui::End();
	// });

	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::animation::executeNextFrame(glfwGetTime()); });
	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::animation::updateDistanceAnimation(); });
	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::collision::resolveCollisions(); });
	ed->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });
	ed->subscribe(PLAYER_DEAD_EVENT, []() {
		std::cout << "STOP APPLICATION\n";
	});
	ed->subscribe(event::loop::LOOP_RENDER, [&igdebug]() {
		if (igdebug->isBoundingBoxVisible())
			systems::render::renderBoundingBox();
	});

	while (!glfwWindowShouldClose(w.getContext())) {
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}

	im.onDetach();
	w.onDetach();

	return 0;
}
