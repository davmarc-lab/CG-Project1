#include "../include/LevelManager.hpp"

using namespace bolt;

const auto em = EntityManager::instance();
const auto defaultShaders = Pair<std::string>{"shader/defaultOrthoVertShader.glsl", "shader/defaultFragShader.glsl"};

MultiMesh LevelManager::createBullet(const vec2 &pos) {
	auto bulletBase = em->createEntity();
	initSquare(bulletBase, {pos.x, pos.y}, {35, 25}, this->m_settings.base, defaultShaders);

	auto bulletPeak = em->createEntity();
	initCircle(bulletPeak, {pos.x - 30, pos.y}, {50, 25}, this->m_settings.peak.mid, this->m_settings.peak.out, defaultShaders, CircleSettings{});

	auto bullet = MultiMesh{{bulletBase, bulletPeak}, static_cast<i32>(bulletBase)};
	Scene::instance()->addEntity(bulletBase);

	return bullet;
}

void LevelManager::createLevel() {
	for (auto i = 0; i < this->m_current; i++) {
		auto b = this->createBullet({1500, 400 / this->m_current * i + 100});
        // i dont like this, copy every time
		EventDispatcher::instance()->subscribe(events::loop::LoopUpdate, [b](auto p) {
			b.transform({-5 * 0.03, 0, 0}, {1, 1, 1}, {0, 0, 0}, 0);
		});
	}
}

void LevelManager::cleanLevel() {}
