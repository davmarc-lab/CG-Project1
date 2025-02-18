#pragma once

#include "../../../Opengl-Core/include/Core.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <glm/trigonometric.hpp>
#include <vector>

class Component {
public:
	Component() = default;

	virtual ~Component() = default;
};

// REMOVE ALL SET AND GET METHODS - USE SYSTEMS
class Transform : public Component {
public:
	inline bool isModelMatrixEnable() const { return this->enableModel; }

	inline void enableModelMatrix(const bool &flag) { this->enableModel = flag; }

	inline const bool &isDirty() const { return this->dirty; }

	inline void setDirty(const bool &dirty) { this->dirty = true; }

	/* // BETTER DON'T FORCE MODEL MATRIX
	inline void setModelMatrix(const glm::mat4 &matrix) {
		this->model = matrix;
		this->dirty = false;
	}
	*/

	inline const glm::vec3 &getPosition() const { return this->position; }

	inline void setPosition(const glm::vec3 &pos) {
		this->position = pos;
		this->dirty = true;
	}

	inline void addPosition(const glm::vec3 &offset) {
		this->position += offset;
		this->dirty = true;
	}

	inline glm::vec3 getScale() const { return this->scale; }

	inline void setScale(const glm::vec3 &scale) {
		this->scale = scale;
		this->dirty = true;
	}

	inline void addScale(const glm::vec3 &offset) {
		this->scale += offset;
		this->dirty = true;
	}

	inline glm::vec3 getRotation() const { return this->rotation; }

	inline void setRotation(const glm::vec3 &rotation) {
		this->rotation = glm::radians(rotation);
		this->dirty = true;
	}

	inline void addRotation(const glm::vec3 &offset) {
		this->rotation += glm::radians(offset);
		this->dirty = true;
	}

	/*
	inline glm::quat getQuaternion() const { return this->quaternion; }

	inline void setQuaternion(const glm::quat &quaternion) {
		this->quaternion = quaternion;
		this->dirty = true;
	}
	*/
	Transform() :
		Component() {
	}

	virtual ~Transform() override = default;

	void updateModelMatrix() {
		if (!this->dirty)
			return;

		glm::mat4 base{1};
		auto t = glm::translate(base, this->position);
		auto s = glm::scale(base, this->scale);

		this->quaternion = glm::quat(this->rotation);
		auto r = glm::toMat4(this->quaternion);

		this->model = t * s * r;
		this->dirty = false;
	}

	glm::vec3 position{};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 rotation{};
	glm::quat quaternion{};

	glm::mat4 model{1};
	bool dirty = true;
	bool enableModel = true;
};

class BufferComponent : public Component {
public:
	BufferComponent() = default;

	virtual ~BufferComponent() override = default;

	ogl::VertexArray vao{};
	ogl::VertexBuffer vbo_g{};
	ogl::VertexBuffer vbo_c{};
	ogl::ElementBuffer ebo{};
};

class VertexComponent : public Component {
public:
	inline std::vector<glm::vec3> getVertexCoords() const { return this->m_vertex; }
	inline void setVertexCoords(const std::vector<glm::vec3> &vertex) { this->m_vertex = vertex; }

	inline void appendVertex(const std::vector<glm::vec3> &vertex) {
		for (auto e : vertex)
			this->m_vertex.push_back(e);
	}

	inline std::vector<glm::vec4> getColorsCoords() const { return this->m_colors; }
	inline void setColorsCoords(const std::vector<glm::vec4> &colors) { this->m_colors = colors; }

	inline void appendColor(const std::vector<glm::vec4> &colors) {
		for (auto e : colors)
			this->m_colors.push_back(e);
	}

	inline std::vector<unsigned int> getIndexCoords() const { return this->m_index; }
	inline void setIndexCoords(const std::vector<unsigned int> &index) { this->m_index = index; }

	inline void appendIndex(const std::vector<unsigned int> &index) {
		for (auto e : index)
			this->m_index.push_back(e);
	}

	VertexComponent(const std::vector<glm::vec3> &vertex, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices) :
		Component() {
		this->m_vertex = vertex;
		this->m_colors = colors;
		this->m_index = indices;
	}

	virtual ~VertexComponent() = default;

private:
	std::vector<glm::vec3> m_vertex{};
	std::vector<glm::vec4> m_colors{};
	std::vector<unsigned int> m_index{};
};

class ShaderComponent : public Component {
public:
	ShaderComponent() :
		Component() {
	}

	virtual ~ShaderComponent() override = default;

private:
	Shared<ogl::ShaderProgram> m_shader{};
};

class RenderComponent : public Component {
public:
	void setRenderCall(const std::function<void()> &func) { this->m_renderCall = std::move(func); }

	void call() {
		if (this->m_renderCall != nullptr)
			this->m_renderCall();
	}

	RenderComponent() :
		Component() {
	}

	virtual ~RenderComponent() override = default;

private:
	std::function<void()> m_renderCall = nullptr;
};

class InputComponent : public Component {
public:
	inline void registerAction(const unsigned int &key, const std::function<void()> &callback) {
		this->callbacks.emplace(key, std::move(callback));
	}

	inline void call(const unsigned int &key) {
		this->callbacks.at(key)();
	}

	inline std::vector<unsigned int> getAllKeys() const {
		std::vector<unsigned int> keys{};
		for (auto [key, _] : this->callbacks) {
			keys.push_back(key);
		}
		return keys;
		// C++20
		// auto kv = std::views::keys(this->callbacks);
		// return std::vector<unsigned int>{kv.begin(), kv.end()};
	}

	InputComponent() = default;

	~InputComponent() override = default;

	std::map<unsigned int, std::function<void()>> callbacks;
};

class AABB : public Component {
public:
	glm::vec3 botLeft{};
	glm::vec3 topRight{};

	void updateCollider(const std::vector<glm::vec3> &coords, const glm::mat4 &model) {
		auto bot = glm::vec3(1, 1, 0);
		bool first = true;

		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				bot = elem;
				first = false;
			}
			bot.x = bot.x >= elem.x ? elem.x : bot.x;
			bot.y = bot.y >= elem.y ? elem.y : bot.y;
		}

		auto top = glm::vec3(1, 1, 0);
		first = true;
		for (auto i = 0; i < coords.size(); i++) {
			auto elem = model * glm::vec4(coords[i], 1);
			if (first) {
				top = elem;
				first = false;
			}
			top.x = top.x <= elem.x ? elem.x : top.x;
			top.y = top.y <= elem.y ? elem.y : top.y;
		}

		this->botLeft = bot;
		this->topRight = top;
	}

	bool isColliding(const AABB &other) const {
		return (this->botLeft.x <= other.topRight.x && this->topRight.x >= other.botLeft.x) &&
			(this->botLeft.y <= other.topRight.y && this->topRight.y >= other.botLeft.y);
	}

	AABB() = default;

	virtual ~AABB() override = default;
};

// Mesh is not a component but an extended Entity
/* class Mesh : public Component {
public:
	Mesh() :
		Component() {}

	virtual ~Mesh() override = default;

private:
	VertexComponent m_vertex{};
	ShaderComponent m_shader{};
	RenderComponent m_render{};
}; */
