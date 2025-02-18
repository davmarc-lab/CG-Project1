#include "../../include/ECS/EcsScene.hpp"

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

	// key element  found
	elem->second.erase(std::find(ALL(elem->second), ett));
}

void BasicScene::removeEntity(const unsigned int &ett) {
	auto rmv = std::vector<unsigned int>{};
	for (auto [key, etts] : this->m_entities) {
		etts.erase(std::find(ALL(etts), ett));
	}
}
