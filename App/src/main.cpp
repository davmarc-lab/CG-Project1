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
// main mesh scene
const auto ecs = BasicScene::instance();

const float WIDTH = 800.f;
const float HEIGHT = 600.f;

const auto ENEMY_SPAWN_DELAY = 5;
const auto ENEMY_MAX_ENTITIES = 5;
// max eneymies per level
const glm::vec4 ENEMY_COLOR = {1, 0, 0, 1};
const float ENEMY_VEL = 3.f;

unsigned int levelCount = 1;

const glm::vec3 PLAYER_VEL = {50, 50, 0};
const glm::vec3 PROJ_OFFSET = {2, 2, 0};
const glm::vec3 HEALTH_BAR_SIZE = {200, 10, 0};
const float HP_FACTOR = 2.f;

const glm::vec4 EYE_COLOR = {0.2274, 0.4627, 0.9411, 1};

/**
 * @brief Player data structure
 */
struct Player {
	/// main mesh id
	unsigned int id;
	/// player gun data
	GunInfo gunInfo{};
	/// gun projectile data
	ProjInfo projInfo{100, PROJ_RANGE, PROJ_COLOR};
	/// is player dead
	bool dead = false;
} player;

/**
 * @brief Retrieves a random position around a point outside a given offset.
 *
 * @param pos the point
 * @param offset the random position offset
 *
 * @return a point in a random position outside the given offset
 */
glm::vec3 getRandomPosNear(const glm::vec3 &pos, const glm::vec3 &offset) {
	auto x = rand() % (int)WIDTH;
	while (x > pos.x - offset.x && x < pos.x + offset.x)
		x = rand() % (int)WIDTH;
	auto y = rand() % (int)HEIGHT;
	while (y > pos.y - offset.y && y < pos.y + offset.y)
		y = rand() % (int)HEIGHT;
	return {x, y, 0};
}

/**
 * @brief Retrieves a random position (z value = 0).
 *
 * @return a random vector
 */
glm::vec3 getRandomPos() {
	auto x = rand() % (int)WIDTH;
	auto y = rand() % (int)HEIGHT;
	return {x, y, 0};
}

/**
 * @brief Checks if a mesh at the given position with the given scale is outside
 * the screen.
 *
 * @param size the window size
 * @param pos the mesh position
 * @param scale the mesh scale
 *
 * @return true if the mesh is outside the window boundaries
 */
bool outOfScreen(const Pair<float> &size, const glm::vec3 &pos, const glm::vec3 &scale) {
	return (pos.y + scale.y > size.y) || (pos.y - scale.y < 0) || (pos.x + scale.x > size.x) || (pos.x - scale.x < 0);
}

/**
 * @brief This method shoots a projectile to the given direction in the given scene.
 * It handles the gun cooldown.
 *
 * @param id the mesh id used to position the projectile
 * @param direction the projectile direction
 * @param scene the scene to draw
 */
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
	// enable stencil buffer to draw mesh outlines https://learnopengl.com/Advanced-OpenGL/Stencil-testing
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	w.addClearMask(GL_STENCIL_BUFFER_BIT);

	// add window function to execute every frame
	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&w]() { w.begin(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	// initialize imgui panel manager
	ImGuiManager im{&w};
	im.onAttach();
	// add imgui operations to execute every frame in different phase
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	// text manager
	const auto tm = TextManager::instance();
	tm->onAttach();

	// level manager
	LevelManager lm{};
	ed->subscribe(event::loop::LOOP_UPDATE, [&lm]() { if (!player.dead) lm.onUpdate(); });

	// creates simple text to keep track of the current level
	TextHelper helper{};
	helper.text = std::string{"Level: " + std::to_string(levelCount)};
	helper.position = {w.getWidth() - 200, w.getHeight() - 50};
	helper.color = {1, 1, 1};
	helper.scale = 1;
	// creates the text
	auto level = tm->addText(helper);

	// when the level is completed executes this operation
	ed->subscribe(EVENT_LEVEL_COMPLETED, [&level, &lm]() {
		level->setText("Level: " + std::to_string(lm.getCurrentLevel()));
	});

	// adds statistics panel
	im.addPanel<ImGuiStats>();

	// creates the shader for the scene
	Shared<ShaderProgram> ss = CreateShared<ShaderProgram>("singlevs.glsl", "singlefs.glsl");
	ss->createShaderProgram();
	ecs->setSingleShader(ss);

	auto ett = EntityManager::instance();

	// create the square where the background is drawn
	auto back = factorySquare({{w.getWidth() / 2, w.getHeight() / 2, -0.9}, {w.getWidth() / 2, w.getHeight() / 2, 0}, {}}, {0.3f, 0.3f, 0.3f, 1.0f});
	ett->removeComponent<Outlined>(back);
	ecs->addEntity(back, ShaderType::SHADER_BACK);

	// creates the body of the player
	auto body = factoryHermite(BasicInfo{{1400, 800, 0.2}, {40, 45, 1}, {}}, "./resources/hermite/player/down.txt", {1, 0.7568, 0.9450, 1});
	// adds a gun
	ett->addComponent<GunComponent>(body, player.gunInfo);
	// adds a collider to the body
	ett->addComponent<AABB>(body);
	ett->addComponent<PlayerComponent>(body);
	// adds the input component for handling user input
	ett->addComponent<InputComponent>(body);
	// adds an health component
	ett->addComponent<HealthComponent>(body);
	// adds a parent component to link multiple meshes to this single mesh
	// Used to manage other meshes relative movement
	ett->addComponent<ParentComponent>(body);
	// adds to the current scene
	ecs->addEntity(body, ShaderType::SHADER_DEFAULT);

	// creates head mesh
	auto head = factoryCircle(BasicInfo{{1399, 824, 0.3}, {27, 20, 1}, {}}, {1, 0.7568, 0.9450, 1}, 40);
	// define head parent mesh for relative movement
	systems::parent::addChild(body, head);
	ecs->addEntity(head, ShaderType::SHADER_DEFAULT);

	// creates the player left eye
	auto leye = factoryCircle(BasicInfo{{1387, 828, 0.4}, {5, 7, 1}, {}}, EYE_COLOR);
	// define left eye parent mesh for relative movement
	systems::parent::addChild(body, leye);
	ecs->addEntity(leye, ShaderType::SHADER_DEFAULT);

	// creates the player right eye
	auto reye = factoryCircle(BasicInfo{{1412, 828, 0.4}, {5, 7, 1}, {}}, EYE_COLOR);
	// define right eye parent mesh for relative movement
	systems::parent::addChild(body, reye);
	ecs->addEntity(reye, ShaderType::SHADER_DEFAULT);

	// creates the player mouth
	auto mouth = factoryHermite(BasicInfo{{1399, 816, 0.4}, {10, 11, 1}, {0, 0, 180}}, "./resources/hermite/mouth.txt");
	// define mouth parent mesh for relative movement
	systems::parent::addChild(body, mouth);
	ecs->addEntity(mouth, ShaderType::SHADER_DEFAULT);

	// creates a generic imgui panel
	auto iglevel = im.addPanel<ImGuiPanel>();
	// define the render function for the imgui panel
	iglevel->setRenderFunc([&iglevel, &lm]() {
		ImGui::Begin("Level", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::Text("Max Enemies: %u", lm.getMaxEnemies());
		ImGui::Text("Spawned Enemies: %u", lm.getEnemiesSpawned());
		ImGui::Text("Current Enemies: %u", lm.getCurrentEnemies());
		ImGui::End();
	});

	// creates the player health bar
	auto healthBar = factorySquare(BasicInfo{{10 + HEALTH_BAR_SIZE.x, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 1}, HEALTH_BAR_SIZE, {}}, {1, 0, 0, 1});
	ecs->addEntity(healthBar, ShaderType::SHADER_DEFAULT);

	// implements health bar behaviour at every frame in the update game loop phase
	// to shrink it when health decrease
	ed->subscribe(event::loop::LOOP_UPDATE, [&healthBar, &body, &w]() {
		auto health = systems::enemy::getHealth(body);
		systems::transform::updatePosition(healthBar, {10 + health * HP_FACTOR, w.getHeight() - HEALTH_BAR_SIZE.y - 10, 0});
		systems::transform::updateScale(healthBar, {health * HP_FACTOR, HEALTH_BAR_SIZE.y, 1});
	});

	// implements all player movement behaviour, storing data in the InputComponent
	systems::input::setKeyCallback(body, GLFW_KEY_W, [&body]() {
		if (!player.dead)
			systems::transform::addPosition(body, 0.1f * PLAYER_VEL * glm::vec3(0, 1, 0));
	});
	systems::input::setKeyCallback(body, GLFW_KEY_A, [&body]() {
		if (!player.dead)
			systems::transform::addPosition(body, 0.1f * PLAYER_VEL * glm::vec3(-1, 0, 0));
	});
	systems::input::setKeyCallback(body, GLFW_KEY_S, [&body]() {
		if (!player.dead)
			systems::transform::addPosition(body, 0.1f * PLAYER_VEL * glm::vec3(0, -1, 0));
	});
	systems::input::setKeyCallback(body, GLFW_KEY_D, [&body]() {
		if (!player.dead)
			systems::transform::addPosition(body, 0.1f * PLAYER_VEL * glm::vec3(1, 0, 0));
	});

	// implements all player shooting behaviour, storing data in the InputComponent
	systems::input::setKeyCallback(body, GLFW_KEY_UP, [&body]() {
		if (!player.dead)
			playerShoot(body, glm::vec3{0, 1, 0}, ecs);
	});
	systems::input::setKeyCallback(body, GLFW_KEY_LEFT, [&body]() {
		if (!player.dead)
			playerShoot(body, glm::vec3{-1, 0, 0}, ecs);
	});
	systems::input::setKeyCallback(body, GLFW_KEY_DOWN, [&body]() {
		if (!player.dead)
			playerShoot(body, glm::vec3{0, -1, 0}, ecs);
	});
	systems::input::setKeyCallback(body, GLFW_KEY_RIGHT, [&body]() {
		if (!player.dead)
			playerShoot(body, glm::vec3{1, 0, 0}, ecs);
	});

	// implements a mouse button callback for the current window, when the left mouse button
	// is clicked, calls the playerShoot() function
	w.setMouseButtonCallback([&body](GLFWwindow *window, int button, int action, int) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			// gets the mouse position
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			// moves the y axis orientation (GLFW y = 0 point is at the top but in the scene
			// y = 0 point is the bottom)
			y = std::abs(y - HEIGHT);
			// calculates the vector which start from the body position and ends at the cursor position
			auto dir = glm::normalize(glm::vec3{x, y, 0} - systems::transform::getPosition(body));
			// shoot to the calculated direction
			playerShoot(body, dir, ecs);
		}
	});

	// these operations are called everytime the gameloop is in update phase
	// they prevent the player to move out of the window
	ed->subscribe(event::loop::LOOP_UPDATE, [&body, &w]() {
		auto pos = systems::transform::getPosition(body);
		auto scale = systems::transform::getScale(body);
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
		systems::transform::updatePosition(body, pos);
	});

	// Entity Manager callbacks
	std::cerr << "Move this operation in EventManger: LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
	// executes all the input callbacks when game loop is in input phase
	ed->subscribe(event::loop::LOOP_INPUT, [&w, &ett]() {
		for (auto e : ett->getEntitiesFromComponent<InputComponent>()) {
			for (auto [key, f] : systems::input::getKeysCallback(e)) {
				if (glfwGetKey(w.getContext(), key) == GLFW_PRESS) {
					f();
				}
			}
		}
	});

	// creates an uniform buffer to send common data to different shaders
	// https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
	UniformBuffer ubo{"Matrices"};
	glm::mat4 proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
	ubo.onAttach();
	// alloc buffer memory for the projection matrix and time variable
	ubo.setup(sizeof(glm::mat4) + sizeof(float) + 12, 0, 0, 0);
	// assign projection matrix values
	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	float time = glfwGetTime();
	// assign time value
	ubo.update(sizeof(glm::mat4), sizeof(float), &time);
	// every time it's time to render meshes it updates the time value
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&ubo]() {
		float time = glfwGetTime();
		ubo.update(sizeof(glm::mat4), sizeof(float), &time);
	});
	// every time the projection matrix is modified updates the matrix values
	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&w, &ubo]() {
		auto proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
		ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	});

	// add general panel to debug collisions (bounding boxes)
	auto igdebug = im.addPanel<ImGuiDebug>();
	igdebug->setRenderFunc([&igdebug]() {
		ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
		auto b = igdebug->isBoundingBoxVisible();
		if (ImGui::Checkbox("View BB", &b)) {
			igdebug->showBoundingBox(b);
		}
		ImGui::Text("Collision count: %zu", systems::collision::getCollisions().size());
		ImGui::End();
	});

	// event for spawning an enemy
	auto offset = glm::vec3{100, 100, 0};
	// everytime the enemy spawn event is "posted" it creates one
	ed->subscribe(EVENT_ENEMY_SPAWN, [&ett, &body, &offset]() {
		if (player.dead)
			return;
		auto pos = systems::transform::getPosition(body);
		auto id = factoryEnemy(BasicInfo{{getRandomPosNear(pos, offset)}, ENEMY_SLIME_SIZE, {}}, ENEMY_COLOR);
		// add a behavior component to simulate enemy movement
		ett->addComponent<BehaviourComponent>(id);

		// enemy moves towards the player
		systems::enemy::setBehaviour(id, [id, body]() {
			auto target = systems::transform::getPosition(body) - systems::transform::getPosition(id);
			auto dir = glm::normalize(target) * ENEMY_VEL;
			dir.z = 0;
			systems::transform::addPosition(id, dir);
		});
		ecs->addEntity(id, ShaderType::SHADER_DEFAULT);
		// increses enemy count for the level manager
	});

	// event to decrease enemy counter
	ed->subscribe(EVENT_ENEMY_DEAD, [&lm]() {
		lm.decreaseEnemies();
	});

	// prevent the level manager to spawn an enemy every frame
	ed->subscribe(EVENT_SPAWN_ITEM, [&lm]() {
		lm.setPause(5);
	});

	// exec all entities behaviour
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::enemy::execAllBehaviourFunc(); });
	// exec all timed animations
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::animation::executeNextFrame(glfwGetTime()); });
	// exec all distance animations for projectiles movement
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::animation::updateDistanceAnimation(); });
	// resolve all entities collisions
	ed->subscribe(event::loop::LOOP_UPDATE, []() { if (!player.dead) systems::collision::resolveCollisions(); });
	// renders all the meshes in the scene
	ed->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });

	// this is executed when the player is dead
	ed->subscribe(PLAYER_DEAD_EVENT, [&w, &tm]() {
		player.dead = true;
		TextHelper h{};
		h.position = {w.getWidth() / 2 - 138, w.getHeight() / 2 + 30};
		h.text = "You are Dead";
		h.color = {1, 1, 1};
		auto l = tm->addText(h);
	});
	// renders bounding boxes if the debug flag is enabled
	ed->subscribe(event::loop::LOOP_RENDER, [&igdebug]() {
		if (igdebug->isBoundingBoxVisible())
			systems::render::renderBoundingBox();
	});

	// Text Rendering on top of all
	ed->subscribe(event::loop::LOOP_RENDER, [&tm]() { tm->onRender(); });

	// compress all entities with ParentComponent bounding boxes
	systems::collision::compressBoundingBox();

	// initialize the stencil buffer to draw a simple outline
	systems::render::initStencilShader();

	while (!glfwWindowShouldClose(w.getContext())) {
		// for each event it executes all the registered callbacks
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}

	// clear all imgui data
	im.onDetach();
	// clear all window data
	w.onDetach();

	return 0;
}
