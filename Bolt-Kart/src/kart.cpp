#include "../../Bolt-Core/include/Engine.hpp"
#include "../../Bolt-Core/include/Graphics.hpp"

#include "../include/HermiteFactory.hpp"
#include "../include/Utils.hpp"

#include <iostream>

using namespace bolt;

#define ROADLIMIT 300

const auto em = EntityManager::instance();
const auto defaultShaders = Pair<std::string>{"shader/defaultOrthoVertShader.glsl", "shader/defaultFragShader.glsl"};
const auto circleHelper = CircleSettings{};

std::string textAmmoPrefix = "Ammo: ";

u32 gameLevel = 0;
u32 finalScore = 0;

u32 playerAmmo = 1;

struct DoubleColor {
	vec4 out{0, 0, 0, 1};
	vec4 mid{1, 1, 1, 1};
};

struct CarSettings {
	DoubleColor back{};
	DoubleColor front{};
	DoubleColor human{{0.94, 0.76, 0.49, 1}, {0.94, 0.76, 0.49, 1}};
	DoubleColor carWheel{{0, 0, 0, 1}, {1, 1, 1, 1}};
	vec4 carBody{1, 0, 0, 1};
	vec4 carWindow{0.78f, 0.96f, 0.94f, 1.f};
	vec3 carVel{7};
} carSettings;

// TODO
struct BulletColor {
	vec4 base{0, 0, 0, 1};
	DoubleColor peak{{0, 0, 0, 1}, {0, 0, 0, 1}};
} bulletsColor;

// TODO
struct BoomerangColor {
	vec4 top{0, 0, 1, 1};
	vec4 bot{0, 0, 1, 1};
} boomerangColor;

MultiMesh createBullet(const vec2 &pos) {
	auto bulletBase = em->createEntity();
	initSquare(bulletBase, {pos.x, pos.y}, {35, 25}, bulletsColor.base, defaultShaders);

	auto bulletPeak = em->createEntity();
	initCircle(bulletPeak, {pos.x - 30, pos.y}, {50, 25}, bulletsColor.peak.mid, bulletsColor.peak.out, defaultShaders, circleHelper);

	auto bullet = MultiMesh{{bulletBase, bulletPeak}, static_cast<i32>(bulletBase)};
	Scene::instance()->addEntity(bulletBase);
	
	return bullet;
}

int main(int argc, char *argv[]) {
	std::cout << "Application started\n";

	WindowProperties properties{};
	properties.maximized = false;
	properties.vsync = true;
	properties.backgroundColor = vec4(0.3, 0.3, 0.3, 1);

	ApplicationSetting settings{};
	settings.type = scene::SCENE_2D;
	settings.name = "Bolt Kart";
	settings.dimension = {1600, 900};
	settings.properties = properties;

	const auto app = CreateUnique<Application>(settings);
	const auto ls = LayerStack::instance();

	const auto w = CreateShared<Window>(settings);
	// it needs to be attached due to UniforBuffer usage
	w->onAttach();
	ls->addCustomLayer(w);

	const auto scene = Scene::instance();
	ls->addCustomLayer(CreateShared<SceneLayer>());

	UniformBuffer ub = UniformBuffer();
	ub.onAttach();
	ub.setup(sizeof(mat4), 0);
	auto proj = Application::getProjectionMatrix();
	ub.update(0, sizeof(mat4), value_ptr(proj));
	EventDispatcher::instance()->subscribe(events::shader::ShaderProjectionChanged, [&proj, &ub](auto &&p) {
		ub.update(0, sizeof(mat4), value_ptr(proj));
	});

	em->subscribeEventCallbacks();

	// road
	auto road = em->createEntity();
	initSquare(road, {0, 0}, {static_cast<f32>(settings.dimension.x), static_cast<f32>(settings.dimension.y)}, {1, 0, 0, 1}, {"shader/defaultOrthoVertShader.glsl", "shader/roadFragmentShader.glsl"});
	// set custom draw call to send needed parameters
	{
		const auto comp = em->getEntityComponent<Mesh>(road);
		const auto vao = comp->vao;
		const auto size = comp->vertices.size();
		const auto shader = em->getEntityComponent<ShaderComponent>(road)->shader.get();
		const auto res = settings.dimension;
		comp->render.setCall([vao, size, shader, res]() {
			shader->use();
			shader->setVec2("resolution", {res.x, res.y - ROADLIMIT});
			shader->setFloat("iTime", Timer::instance()->getTimef());
			RenderApi::instance()->getRenderer()->drawArraysTriangles(vao, size, 0);
		});
	}
	scene->addEntity(road);

	// goal
	auto star = em->createEntity();
	auto tmp = readDataFromFile("resources/hermite/star.txt");
	buildHermite(vec4(1, 1, 0, 1), vec4(1, 1, 0, 1), tmp.get());
	if (tmp != nullptr) {
		initHermiteMesh(star, tmp.get());
		auto comp = em->getEntityComponent<Transform>(star);
		comp->setPosition({1500, 100, 0});
		comp->setScale({40, 40, 1});
		scene->addEntity(star);
	}

	// car
	auto carBody = em->createEntity();
	initSquare(carBody, {153, 140}, {89, 40}, carSettings.carBody, defaultShaders);
	auto input = em->addComponent<InputComponent>(carBody);

	auto carWindow = em->createEntity();
	initSquare(carWindow, {170, 150}, {40, 20}, carSettings.carWindow, defaultShaders);

	auto fwheel = em->createEntity();
	initCircle(fwheel, {200, 100}, {20, 20}, carSettings.front.mid, carSettings.front.out, defaultShaders, circleHelper);

	auto rwheel = em->createEntity();
	initCircle(rwheel, {100, 100}, {20, 20}, carSettings.front.mid, carSettings.front.out, defaultShaders, circleHelper);

	auto wheel = em->createEntity();
	initCircle(wheel, {204, 143}, {10, 10}, carSettings.carWheel.mid, carSettings.carWheel.out, defaultShaders, circleHelper);
	{
		auto comp = em->getEntityComponent<Transform>(wheel);
		comp->setRotation({0, 15, 15});
	}

	MultiMesh car = {{carBody, carWindow, fwheel, rwheel, wheel}, static_cast<i32>(carBody)};
	auto dt = 0.01f;
	input->registerAction(GLFW_KEY_W, [&car, dt]() {
		car.transform(carSettings.carVel * vec3(0, 1, 0) * dt, {1, 1, 1}, {0, 0, 0}, 0);
	});
	input->registerAction(GLFW_KEY_S, [&car, dt]() {
		car.transform(carSettings.carVel * vec3(0, -1, 0) * dt, {1, 1, 1}, {0, 0, 0}, 0);
	});
	input->registerAction(GLFW_KEY_A, [&car, dt]() {
		car.transform(carSettings.carVel * vec3(-1, 0, 0) * dt, {1, 1, 1}, {0, 0, 0}, 0);
	});
	input->registerAction(GLFW_KEY_D, [&car, dt]() {
		car.transform(carSettings.carVel * vec3(1, 0, 0) * dt, {1, 1, 1}, {0, 0, 0}, 0);
	});

	auto b = createBullet(vec2(1600, 400));

	// add human?????

	// Prepare all events
	auto ed = EventDispatcher::instance();
	ed->subscribe(events::loop::LoopUpdate, [&b](auto p) {
		b.transform({-5 * 0.03, 0, 0}, {1, 1, 1}, {0, 0, 0}, 0);
	});

	// Start application
	app->run();

	std::cout << "Application closed\n";
	return 0;
}
