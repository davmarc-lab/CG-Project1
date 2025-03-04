#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include "../include/AppGui.hpp"
#include "../include/Factory.hpp"

#include "../include/ECS/Component.hpp"
#include "../include/ECS/Ett.hpp"
#include "../include/ECS/System.hpp"

#include "../include/LevelManager.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../include/ECS/EcsScene.hpp"
#include "../include/HermiteFactory.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto ecs = BasicScene::instance();

const float WIDTH = 1600.f;
const float HEIGHT = 900.f;

const auto ENEMY_SPAWN_DELAY = 5;
const auto ENEMY_MAX_ENTITIES = 5;
const glm::vec4 ENEMY_COLOR = {1, 0, 0, 1};
const float ENEMY_VEL = 3.f;

float lastEnemySpawnTime = 0;
unsigned int enemyCount = 0;

unsigned int levelCount = 1;

const glm::vec3 PLAYER_VEL = {50, 50, 0};
const glm::vec3 PROJ_OFFSET = {2, 2, 0};
const glm::vec3 HEALTH_BAR_SIZE = {200, 10, 0};
const float HP_FACTOR = 2.f;

const glm::vec4 EYE_COLOR = {0.2274, 0.4627, 0.9411, 1};

struct Player {
	unsigned int id;
	GunInfo gunInfo{};
	ProjInfo projInfo{100, PROJ_RANGE, PROJ_COLOR};
} player;

glm::vec3 getRandomPos() {
	auto x = rand() % (int)WIDTH;
	auto y = rand() % (int)HEIGHT;
	return {x, y, 0};
}

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
	srand(time(NULL));

	WindowSettings s{};
	s.decorated = false;
	s.vsync = true;
	s.size = {WIDTH, HEIGHT};
	s.bgColor = {0};
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

	const auto tm = TextManager::instance();
	tm->onAttach();

	LevelManager lm{};
	ed->subscribe(event::loop::LOOP_UPDATE, [&lm]() { lm.onUpdate(); });

	TextHelper helper{};
	helper.text = std::string{"Level: " + std::to_string(levelCount)};
	helper.position = {w.getWidth() - 200, w.getHeight() - 50};
	helper.color = {1, 1, 1};
	helper.scale = 1;
	auto level = tm->addText(helper);

	ed->subscribe(EVENT_LEVEL_COMPLETED, [&level, &lm]() {
		level->setText("Level: " + std::to_string(lm.getCurrentLevel()));
	});

	im.addPanel<ImGuiStats>();

	Shared<ShaderProgram> shader = CreateShared<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
	shader->createShaderProgram();
	Shared<ShaderProgram> backShader = CreateShared<ShaderProgram>("backVertShader.glsl", "backFragShader.glsl");
	backShader->createShaderProgram();

	auto ett = EntityManager::instance();

	auto back = factorySquare({{w.getWidth() / 2, w.getHeight() / 2, 0}, {w.getWidth() / 2, w.getHeight() / 2, 0}, {}}, {0.3f, 0.3f, 0.3f, 1.0f});
	ecs->addEntity(backShader, back);

	auto first = factoryHermite(BasicInfo{{1400, 800, 0}, {40, 45, 1}, {}}, "./resources/hermite/player/down.txt", {1, 0.7568, 0.9450, 1});
	ecs->addEntity(shader, first);
	ett->addComponent<GunComponent>(first, player.gunInfo);
	ett->addComponent<PlayerComponent>(first);
	ett->addComponent<HealthComponent>(first);
	ett->addComponent<ParentComponent>(first);

	auto head = factoryCircle(BasicInfo{{1399, 824, 1}, {27, 20, 1}, {}}, {1, 1, 0, 1}, 40);
	ett->addComponent<AABB>(head);
	systems::parent::addChild(first, head);
	ecs->addEntity(shader, head);

	auto leye = factoryCircle(BasicInfo{{1387, 828, 0}, {5, 7, 1}, {}}, EYE_COLOR);
	systems::parent::addChild(first, leye);
	ecs->addEntity(shader, leye);
	auto reye = factoryCircle(BasicInfo{{1412, 828, 0}, {5, 7, 1}, {}}, EYE_COLOR);
	systems::parent::addChild(first, reye);
	ecs->addEntity(shader, reye);

	// auto pr = factoryHermite(BasicInfo{{}, {}, {}}, "hermite/mouth.txt", {});

	auto iglevel = im.addPanel<ImGuiPanel>();
	iglevel->setRenderFunc([&iglevel, &lm]() {
		ImGui::Begin("Level");
		ImGui::Text("Max Enemies: %u", lm.getMaxEnemies());
		ImGui::Text("Spawned Enemies: %u", lm.getEnemiesSpawned());
		ImGui::Text("Current Enemies: %u", lm.getCurrentEnemies());
		ImGui::End();
	});

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
	systems::input::setKeyCallback(first, GLFW_KEY_UP, [&first, shader]() {
		playerShoot(first, glm::vec3{0, 1, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_LEFT, [&first, shader]() {
		playerShoot(first, glm::vec3{-1, 0, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_DOWN, [&first, shader]() {
		playerShoot(first, glm::vec3{0, -1, 0}, ecs, shader);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_RIGHT, [&first, shader]() {
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
	ubo.setup(sizeof(glm::mat4) + sizeof(float) + 12, 0, 0, 0);
	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	float time = glfwGetTime();
	ubo.update(sizeof(glm::mat4), sizeof(float), &time);
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&ubo]() {
		float time = glfwGetTime();
		ubo.update(sizeof(glm::mat4), sizeof(float), &time);
	});
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

	// spawn enemy if they are less then MAX_ENENMIES, if not they spwan after ENEMY_DELAY seconds
	// ed->subscribe(event::loop::LOOP_END_RENDER, []() {
	// 	auto time = glfwGetTime();
	// 	if (enemyCount <= ENEMY_MAX_ENTITIES) {
	// 		ed->post(EVENT_ENEMY_SPAWN);
	// 	} else {
	// 		if (time - lastEnemySpawnTime > ENEMY_SPAWN_DELAY) {
	// 			lastEnemySpawnTime = time;
	// 			ed->post(EVENT_ENEMY_SPAWN);
	// 		}
	// 	}
	// });

	// event for spawning an enemy
	ed->subscribe(EVENT_ENEMY_SPAWN, [&ett, &shader, &first]() {
		auto id = factoryEnemy(BasicInfo{{getRandomPos()}, ENEMY_SLIME_SIZE, {}}, ENEMY_COLOR);
		ett->addComponent<BehaviourComponent>(id);
		systems::enemy::setBehaviour(id, [id, first]() {
			auto target = systems::transform::getPosition(first) - systems::transform::getPosition(id);
			systems::transform::addPosition(id, glm::normalize(target) * ENEMY_VEL);
		});
		ecs->addEntity(shader, id);
		enemyCount++;
	});

	// event to decrease enemy counter
	ed->subscribe(EVENT_ENEMY_DEAD, [&lm]() {
		enemyCount--;
		lm.decreaseEnemies();
	});

	ed->subscribe(event::loop::LOOP_UPDATE, []() { systems::enemy::execAllBehaviourFunc(); });
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

	// Text Rendering on top of all
	ed->subscribe(event::loop::LOOP_RENDER, [&tm]() { tm->onRender(); });

	// compress all BoundingBox
	systems::collision::compressBoundingBox();

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
