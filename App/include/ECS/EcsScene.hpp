#pragma once

#include <map>

#include <vector>

#include "../../../Opengl-Core/include/Shader/Shader.hpp"

enum ShaderType {
	SHADER_BACK,
	SHADER_DEFAULT,
	SHADER_STENCIL,
	SHADER_TEXT,
};

class BasicScene {
public:
	void setSingleShader(const Shared<ogl::ShaderProgram> &shader);
	Shared<ogl::ShaderProgram> getShader() const { return this->m_shader; }
	void addEntity(const unsigned int &ett, const ShaderType &type);
	void removeEntity(const ShaderType &shader, const unsigned int &ett);
	void removeEntity(const unsigned int &ett);

	std::map<ShaderType, std::vector<unsigned int>> getShaderEntityMap() const { return this->m_entities; }

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
	std::map<ShaderType, std::vector<unsigned int>> m_entities{};
	Shared<ogl::ShaderProgram> m_shader = nullptr;
	bool m_init = false;

	inline static Shared<BasicScene> s_pointer = nullptr;

	BasicScene() = default;
};
