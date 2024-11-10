#include "../../../include/Graphic/Buffer/VertexBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"
#include <iostream>

namespace bolt {
	void VertexBuffer::onAttach() {
		glGenBuffers(1, &this->m_id);
	}

	void VertexBuffer::onDetach() {
		glDeleteBuffers(1, &this->m_id);
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::setup(const f32 *vertices, const i64 &size, const u32 &usage) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, usage == 0 ? GL_STATIC_DRAW : usage);
	}

	template <typename T>
	void VertexBuffer::setup(const T *vertices, const i64 &size, const u32 &usage) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, usage == 0 ? GL_STATIC_DRAW : usage);
	}

	void VertexBuffer::setup(const std::vector<vec2> &vertices, const u32 &usage) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), vertices.data(), usage == 0 ? GL_STATIC_DRAW : usage);
	}

	void VertexBuffer::setup(const std::vector<vec3> &vertices, const u32 &usage) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), usage == 0 ? GL_STATIC_DRAW : usage);
	}

	void VertexBuffer::setup(const std::vector<vec4> &vertices, const u32 &usage) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec4), vertices.data(), usage == 0 ? GL_STATIC_DRAW : usage);
	}

	template <typename T>
	void VertexBuffer::setupSubData(const T *vertices, const i64 &size, const i64 &offset) {
		this->bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	template <typename T>
	void VertexBuffer::setupSubData(const std::vector<T> &vertices, const i64 &offset) {
		this->bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(T), vertices.data());
	}
}
