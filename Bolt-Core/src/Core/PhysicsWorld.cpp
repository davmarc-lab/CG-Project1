#include "../../include/Core/Physics.hpp"

#include "../../include/ECS/EntityManager.hpp"

namespace bolt {
	inline static auto em = EntityManager::instance();
	inline f32 density = 1.f;

	void PhysicsWorld::addEntity(const u32& id) {
		auto physic = em->getEntityComponent<PhysicComponent>(id);
		auto model = em->getEntityComponent<Transform>(id);
		physic->mass = model->getScale().x * model->getScale().y * density;
		this->m_entities.insert(id);
	}

	void PhysicsWorld::onAttach() {
		auto ids = em->getEntitiesFromComponent<PhysicComponent>();
		for (const auto& id : ids) {
			this->m_entities.insert(id);
		}
        this->m_attached = true;
	}

	void PhysicsWorld::onDetach() {}

	void PhysicsWorld::onUpdate() {
		this->m_currTime = this->m_time.getTime();
		this->m_frameTime = this->m_currTime - this->m_prevTime;
		this->m_prevTime = this->m_currTime;

		this->step();
	}

	void PhysicsWorld::step() {
		for (auto id : this->m_entities) {
			auto model = em->getEntityComponent<Transform>(id);
			auto physic = em->getEntityComponent<PhysicComponent>(id);

			// solver
			physic->force += physic->mass * this->m_gravity;
			physic->velocity += physic->force / physic->mass * this->m_frameTime;
			model->addPosition(physic->velocity * this->m_frameTime);
			physic->force = vec3(0);
		}
	}
} // namespace bolt
