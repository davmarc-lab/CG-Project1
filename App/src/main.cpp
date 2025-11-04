#include "../../Opengl-Core/include/Core.hpp"
#include "../../Opengl-Core/include/Graphic.hpp"

#include "../include/AppGui.hpp"
#include "../include/Factory.hpp"

#include "../include/ECS/Component.hpp"
#include "../include/ECS/Ett.hpp"
#include "../include/ECS/System.hpp"

#include "../include/LevelManager.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/ECS/EcsScene.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto ecs = BasicScene::instance();

const float WIDTH = 800.f;
const float HEIGHT = 600.f;

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
	bool dead = false;
} player;

glm::vec3 getRandomPosNear(const glm::vec3 &pos, const glm::vec3 &offset) {
	auto x = rand() % (int)WIDTH;
	while (x > pos.x - offset.x && x < pos.x + offset.x)
		x = rand() % (int)WIDTH;
	auto y = rand() % (int)HEIGHT;
	while (y > pos.y - offset.y && y < pos.y + offset.y)
		y = rand() % (int)HEIGHT;
	return {x, y, 0};
}

glm::vec3 getRandomPos() {
	auto x = rand() % (int)WIDTH;
	auto y = rand() % (int)HEIGHT;
	return {x, y, 0};
}

bool outOfScreen(const Pair<float> &size, const glm::vec3 &pos, const glm::vec3 &scale) {
	return (pos.y + scale.y > size.y) || (pos.y - scale.y < 0) || (pos.x + scale.x > size.x) || (pos.x - scale.x < 0);
}

void playerShoot(unsigned int &id, const glm::vec3 &direction, const Shared<BasicScene> &scene) {
	if (direction == glm::vec3(0))
		return;

	ASSERT(EntityManager::instance()->entityHasComponent<GunComponent>(id));

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
		scene->addEntity(factoryProjectile(BasicInfo{{pos + offset}, {PROJ_SIZE}}, player.projInfo.color, player.projInfo, direction), ShaderType::SHADER_DEFAULT);
		systems::gun::updateLastShoot(id, glfwGetTime());
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	WindowSettings s{};
	s.position = {40, 40};
	s.decorated = false;
	s.vsync = true;
	s.size = {WIDTH, HEIGHT};
	s.bgColor = {0};
	Window w{s};
	w.onAttach();
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	w.addClearMask(GL_STENCIL_BUFFER_BIT);

	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&w]() { w.begin(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	ImGuiManager im{&w};
	im.onAttach();
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	const auto tm = TextManager::instance();
	tm->onAttach();

	LevelManager lm{};
	ed->subscribe(event::loop::LOOP_UPDATE, [&lm]() { if (!player.dead) lm.onUpdate(); });

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

	Shared<ShaderProgram> ss = CreateShared<ShaderProgram>("singlevs.glsl", "singlefs.glsl");
	ss->createShaderProgram();
	ecs->setSingleShader(ss);

	auto ett = EntityManager::instance();

	auto back = factorySquare({{w.getWidth() / 2, w.getHeight() / 2, -0.9}, {w.getWidth() / 2, w.getHeight() / 2, 0}, {}}, {0.3f, 0.3f, 0.3f, 1.0f});
	ett->removeComponent<Outlined>(back);
	ecs->addEntity(back, ShaderType::SHADER_BACK);

	auto first = factoryHermite(BasicInfo{{1400, 800, 0.2}, {40, 45, 1}, {}}, "./resources/hermite/player/down.txt", {1, 0.7568, 0.9450, 1});
	ecs->addEntity(first, ShaderType::SHADER_DEFAULT);
	ett->addComponent<GunComponent>(first, player.gunInfo);
	ett->addComponent<PlayerComponent>(first);
	ett->addComponent<InputComponent>(first);
	ett->addComponent<HealthComponent>(first);
	ett->addComponent<ParentComponent>(first);

	auto head = factoryCircle(BasicInfo{{1399, 824, 0.3}, {27, 20, 1}, {}}, {1, 1, 0, 1}, 40);
	ett->addComponent<AABB>(head);
	systems::parent::addChild(first, head);
	ecs->addEntity(head, ShaderType::SHADER_DEFAULT);

	auto leye = factoryCircle(BasicInfo{{1387, 828, 0.4}, {5, 7, 1}, {}}, EYE_COLOR);
	systems::parent::addChild(first, leye);
	ecs->addEntity(leye, ShaderType::SHADER_DEFAULT);
	auto reye = factoryCircle(BasicInfo{{1412, 828, 0.4}, {5, 7, 1}, {}}, EYE_COLOR);
	systems::parent::addChild(first, reye);
	ecs->addEntity(reye, ShaderType::SHADER_DEFAULT);
	auto mouth = factoryHermite(BasicInfo{{1399, 816, 0.4}, {10, 11, 1}, {0, 0, 180}}, "./resources/hermite/mouth.txt");
	systems::parent::addChild(first, mouth);
	ecs->addEntity(mouth, ShaderType::SHADER_DEFAULT);

	auto iglevel = im.addPanel<ImGuiPanel>();
	iglevel->setRenderFunc([&iglevel, &lm]() {
		ImGui::Begin("Level");
		ImGui::Text("Max Enemies: %u", lm.getMaxEnemies());
		ImGui::Text("Spawned Enemies: %u", lm.getEnemiesSpawned());
		ImGui::Text("Current Enemies: %u", lm.getCurrentEnemies());
		ImGui::End();
	});

	auto healthBar = factorySquare(BasicInfo{{10 + HEALTH_BAR_SIZE.x, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 1}, HEALTH_BAR_SIZE, {}}, {1, 0, 0, 1});
	ecs->addEntity(healthBar, ShaderType::SHADER_DEFAULT);

	ed->subscribe(event::loop::LOOP_UPDATE, [&healthBar, &first, &w]() {
		auto health = systems::enemy::getHealth(first);
		systems::transform::updatePosition(healthBar, {10 + health * HP_FACTOR, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 0});
		systems::transform::updateScale(healthBar, {health * HP_FACTOR, HEALTH_BAR_SIZE.y, 1});
	});

	// movement
	systems::input::setKeyCallback(first, GLFW_KEY_W, [&first]() {
		if (!player.dead)
			systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(0, 1, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_A, [&first]() {
		if (!player.dead)
			systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(-1, 0, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_S, [&first]() {
		if (!player.dead)
			systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(0, -1, 0));
	});
	systems::input::setKeyCallback(first, GLFW_KEY_D, [&first]() {
		if (!player.dead)
			systems::transform::addPosition(first, 0.1f * PLAYER_VEL * glm::vec3(1, 0, 0));
	});

	// shoot
	systems::input::setKeyCallback(first, GLFW_KEY_UP, [&first]() {
		if (!player.dead)
			playerShoot(first, glm::vec3{0, 1, 0}, ecs);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_LEFT, [&first]() {
		if (!player.dead)
			playerShoot(first, glm::vec3{-1, 0, 0}, ecs);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_DOWN, [&first]() {
		if (!player.dead)
			playerShoot(first, glm::vec3{0, -1, 0}, ecs);
	});
	systems::input::setKeyCallback(first, GLFW_KEY_RIGHT, [&first]() {
		if (!player.dead)
			playerShoot(first, glm::vec3{1, 0, 0}, ecs);
	});

	w.setMouseButtonCallback([&first](GLFWwindow *window, int button, int action, int) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			y = std::abs(y - HEIGHT);
			auto dir = glm::normalize(glm::vec3{x, y, 0} - systems::transform::getPosition(first));
			playerShoot(first, dir, ecs);
		}
	});

	ed->subscribe(event::loop::LOOP_UPDATE, [&first, &w]() {
		auto pos = systems::transform::getPosition(first);
		auto scale = systems::transform::getScale(first);
		auto z = pos.z;
		if (pos.y + scale.y > w.getHeight()) {
			pos = {pos.x, w.getHeight() - scale.y, z};
		}
		if (pos.y - scale.y < 0) {
			pos = {pos.x, scale.y, z};
		}
		if (pos.x - scale.x < 0) {
			pos = {scale.x, pos.y, z};
		}
		if (pos.x + scale.x > w.getWidth()) {
			pos = {w.getWidth() - scale.x, pos.y, z};
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

	// event for spawning an enemy
	auto offset = glm::vec3{100, 100, 0};
	ed->subscribe(EVENT_ENEMY_SPAWN, [&ett, &first, &offset]() {
		return;
		if (player.dead)
			return;
		auto pos = systems::transform::getPosition(first);
		auto id = factoryEnemy(BasicInfo{{getRandomPosNear(pos, offset)}, ENEMY_SLIME_SIZE, {}}, ENEMY_COLOR);
		ett->addComponent<BehaviourComponent>(id);
		systems::enemy::setBehaviour(id, [id, first]() {
			auto target = systems::transform::getPosition(first) - systems::transform::getPosition(id);
			systems::transform::addPosition(id, glm::normalize(target) * ENEMY_VEL);
		});
		ecs->addEntity(id, ShaderType::SHADER_DEFAULT);
		enemyCount++;
	});

	// event to decrease enemy counter
	ed->subscribe(EVENT_ENEMY_DEAD, [&lm]() {
		enemyCount--;
		lm.decreaseEnemies();
	});

	ed->subscribe(EVENT_SPAWN_ITEM, [&lm]() {
		lm.setPause(5);
	});

	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::enemy::execAllBehaviourFunc(); });
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::animation::executeNextFrame(glfwGetTime()); });
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::animation::updateDistanceAnimation(); });
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::collision::resolveCollisions(); });
	ed->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });

	ed->subscribe(PLAYER_DEAD_EVENT, [&w, &tm]() {
		player.dead = true;
		TextHelper h{};
		h.position = {w.getWidth() / 2 - 138, w.getHeight() / 2 + 30};
		h.text = "You are Dead";
		h.color = {1, 1, 1};
		auto l = tm->addText(h);
	});
	ed->subscribe(event::loop::LOOP_RENDER, [&igdebug]() {
		if (igdebug->isBoundingBoxVisible())
			systems::render::renderBoundingBox();
	});

	// Text Rendering on top of all
	ed->subscribe(event::loop::LOOP_RENDER, [&tm]() { tm->onRender(); });

	// compress all BoundingBox
	systems::collision::compressBoundingBox();

	systems::render::initStencilShader();

	while (!glfwWindowShouldClose(w.getContext())) {
		ed->post(event::loop::LOOP_INPUT);
		// if (!player.dead) {
		ed->post(event::loop::LOOP_UPDATE);
		// }
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}

	im.onDetach();
	w.onDetach();

	return 0;
}
