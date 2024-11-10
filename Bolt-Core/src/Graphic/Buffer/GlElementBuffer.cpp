#include "../../../include/Graphic/Buffer/ElementBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

namespace bolt {
	ElementBuffer::~ElementBuffer() {
		if (this->m_id)
			glDeleteBuffers(1, &this->m_id);
	}

	void ElementBuffer::onAttach() {
		glGenBuffers(1, &this->m_id);
	}

	void ElementBuffer::onDetach() {
		glDeleteBuffers(1, &this->m_id);
	}

	void ElementBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
	}

	void ElementBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ElementBuffer::setup(const f32* indices, const i64& size, const u32& usage) const {
		this->bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage == 0 ? GL_STATIC_DRAW : usage);
	}

	template <typename T>
	void ElementBuffer::setup(const T* indices, const i64& size, const u32& usage) {
		this->bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage == 0 ? GL_STATIC_DRAW : usage);
	}

	template <typename T>
	void ElementBuffer::setup(const std::vector<T>& indices, const u32& usage) {
		this->bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), indices.data(), usage);
	}
} // namespace Bolt
