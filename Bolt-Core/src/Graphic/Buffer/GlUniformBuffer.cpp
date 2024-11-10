#include "../../../include/Graphic/Buffer/UniformBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"
#include <iostream>

namespace bolt {
	void UniformBuffer::onAttach() {
		glGenBuffers(1, &this->m_id);
	}

	void UniformBuffer::onDetach() {
		glDeleteBuffers(1, &this->m_id);
	}

	void UniformBuffer::bind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, this->m_id);
	}

	void UniformBuffer::unbind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::setup(const u32& size, const u32& binding, const u32& offset, const u32& rangeSize) {
		this->m_memAllocated = size;
		this->bind();
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		if (!(offset && rangeSize))
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->m_id);
		else
			glBindBufferRange(GL_UNIFORM_BUFFER, binding, this->m_id, offset, rangeSize);
		this->unbind();
	}

	void UniformBuffer::fastSetup(const u32& size, const u32& binding, const u32& offset, const u32& rangeSize) {
		this->m_memAllocated = size;
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		if (!(offset && rangeSize))
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->m_id);
		else
			glBindBufferRange(GL_UNIFORM_BUFFER, binding, this->m_id, offset, rangeSize);
	}

	void UniformBuffer::update(const u32& offset, const u32& size, void* data) {
		this->m_memAllocated = size;
		this->bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		this->unbind();
	}

	void UniformBuffer::fastUpdate(const u32& offset, const u32& size, void* data) {
		this->m_memAllocated = size;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
} // namespace bolt
