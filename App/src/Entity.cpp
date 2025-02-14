#include "../include/Entity.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/trigonometric.hpp>

const glm::mat4 base{1};

void EntityModel::updateModelMatrix() {
	auto t = glm::translate(base, this->position);
	auto s = glm::scale(base, this->scale / glm::vec3(2, 2, 1));
	// this->quat = glm::quat(glm::radians(this->rotation));
	auto r = glm::toMat4(this->quat);

	this->model = (t * s * r);
}

void Entity::init() {
	if (this->m_init) return;

	this->m_buffers.vao.onAttach();
	this->m_buffers.vao.bind();
	this->m_buffers.vbo_g.onAttach();
	this->m_buffers.vbo_g.setup(this->m_vertex.coords.data(), this->m_vertex.coords.size(), GL_STATIC_DRAW);
	this->m_buffers.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (!this->m_vertex.colors.empty()) {
		this->m_buffers.vbo_c.onAttach();
		this->m_buffers.vbo_c.setup(this->m_vertex.colors.data(), this->m_vertex.colors.size(), GL_STATIC_DRAW);
		this->m_buffers.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if (!this->m_vertex.indices.empty()) {
		this->m_buffers.ebo.onAttach();
		this->m_buffers.ebo.setup(this->m_vertex.indices.data(), this->m_vertex.indices.size(), GL_STATIC_DRAW);
	}

	this->m_init = true;
}
