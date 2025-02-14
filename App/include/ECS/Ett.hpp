#pragma once

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class EntityManager {
public:
	using Index = unsigned int;

	Index createEntity() {
		auto id = this->m_currentId;
		this->m_currentId++;

		this->m_entities.push_back(id);
		this->m_ettComponent.emplace(id, std::vector<Shared<Component>>{});

		return id;
	}

	bool removeEntity(const Index &id);

	inline bool isEntityValid(const Index &id) { return this->m_ettComponent.count(id); }

	template <typename T, typename... Args>
	inline Shared<T> addComponent(const Index &id, Args &&...args) {
		if (!this->isEntityValid(id))
			return nullptr;

		auto comp = CreateShared<T>(std::forward<Args>(args)...);
		this->m_ettComponent.at(id).emplace_back(comp);

		auto name = CLASSNAME(T);
		if (this->m_compEntity.count(name)) {
			this->m_compEntity.at(name).push_back(id);
		} else {
			this->m_compEntity.emplace(name, std::vector<Index>{});
			this->m_compEntity.at(name).push_back(id);
		}
		return comp;
	}

	template <typename T>
	inline std::vector<Index> getEntitiesFromComponent() const {
		if (!this->m_compEntity.count(CLASSNAME(T)))
			return {};

		return this->m_compEntity.at(CLASSNAME(T));
	}

	template <typename T>
	inline Shared<T> getComponentFromId(const Index &id) {
		if (!this->m_compEntity.count(CLASSNAME(T)))
			return nullptr;

		// the entity with id hasn't this component.
		if (std::find(ALL(this->m_compEntity.at(CLASSNAME(T))), id) == this->m_compEntity.at(CLASSNAME(T)).end()) {
			std::cerr << "The entity with id (" << id << ") doesn't have this component (" << CLASSNAME(T) << ").\n";
			return nullptr;
		}

        // finds T component by using dynamic cast.
		return std::dynamic_pointer_cast<T>(*std::find_if(ALL(this->m_ettComponent.at(id)), [](auto e) {
			return std::dynamic_pointer_cast<T>(e) != nullptr;
		}));
	}

	EntityManager(EntityManager &other) = delete;

	void operator=(const EntityManager &other) = delete;

	inline static Shared<EntityManager> instance() {
		if (s_pointer == nullptr) {
			Shared<EntityManager> copy(new EntityManager());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

private:
	Index m_currentId = 0;

	std::vector<Index> m_entities{};
	std::map<Index, std::vector<Shared<Component>>> m_ettComponent{};
	std::map<std::string, std::vector<Index>> m_compEntity{};

	inline static Shared<EntityManager> s_pointer = nullptr;

	EntityManager() = default;
};
