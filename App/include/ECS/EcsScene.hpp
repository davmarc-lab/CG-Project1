#pragma once

#include <unordered_map>

#include <vector>

#include "../../../Opengl-Core/include/Shader/Shader.hpp"

class BasicScene {
public:
	void addEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);
	void removeEntity(Shared<ogl::ShaderProgram> &shader, const unsigned int &ett);
	void removeEntity(const unsigned int &ett);

	std::unordered_map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> getShaderEntityMap() const { return this->m_entities; }

	BasicScene(BasicScene &other) = delete;

	void operator=(const BasicScene &other) = delete;

	inline static Shared<BasicScene> instance() {
		if (s_pointer == nullptr) {
			Shared<BasicScene> copy(new BasicScene());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	~BasicScene() = default;

private:
	std::unordered_map<Shared<ogl::ShaderProgram>, std::vector<unsigned int>> m_entities{};

	inline static Shared<BasicScene> s_pointer = nullptr;

	BasicScene() = default;
};
