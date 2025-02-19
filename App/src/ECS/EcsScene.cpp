#include "../../include/ECS/EcsScene.hpp"

#include <algorithm>

void BasicScene::addEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett) {
	if (this->m_entities.find(shader) == this->m_entities.end()) {
		this->m_entities.insert_or_assign(shader, std::vector<unsigned int>{});
	}
	this->m_entities.at(shader).push_back(ett);
}

void BasicScene::removeEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett) {
	auto elem = this->m_entities.find(shader);
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
