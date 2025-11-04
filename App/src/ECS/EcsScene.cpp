#include "../../include/ECS/EcsScene.hpp"

#include <algorithm>

void BasicScene::setSingleShader(const Shared<ogl::ShaderProgram> &shader) {
	this->m_init = true;
}

void BasicScene::addEntity(const unsigned int &ett, const ShaderType &type) {
	ASSERT(this->m_init);
	if (this->m_entities.find(type) == this->m_entities.end()) {
		this->m_entities.insert_or_assign(type, std::vector<unsigned int>{});
	}
	this->m_entities.at(type).push_back(ett);
}

void BasicScene::removeEntity(const ShaderType &type, const unsigned int &ett) {
	auto elem = this->m_entities.find(type);
	if (elem == this->m_entities.end()) {
		this->removeEntity(ett);
		return;
	}

	// key element found
	elem->second.erase(std::find(ALL(elem->second), ett));
}

void BasicScene::removeEntity(const unsigned int &ett) {
	for (auto it = this->m_entities.begin(); it != this->m_entities.end();) {
		auto &ids = it->second;
		ids.erase(std::remove(ALL(ids), ett), ids.end());

		if (ids.empty())
			it = this->m_entities.erase(it);
		else
			++it;
	}
}
