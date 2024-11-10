#include "../../../include/Graphic/Buffer/VertexArray.hpp"

#include "../../../include/Graphic/Buffer/VertexBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

#include <memory>
#include <iostream>

namespace bolt {
	void VertexArray::onAttach() {
		glGenVertexArrays(1, &this->m_id);
	}

	void VertexArray::onDetach() {
		glDeleteVertexArrays(1, &this->m_id);
	}

	void VertexArray::bind() const {
		glBindVertexArray(this->m_id);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::linkVertexBuffer(const VertexBuffer& vbo, const u32& layout, const i32& size, const u32& type, const i32& stride, void* offset, const b8& normalize) const {
		this->bind();
		vbo.bind();
		glVertexAttribPointer(layout, size, type == 0 ? GL_FLOAT : type, normalize ? GL_TRUE : GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
	}

	void VertexArray::linkAttribFast(u32 layout, i32 size, u32 type, b8 normalize, i32 stride, void* offset) const {
		glVertexAttribPointer(layout, size, type == 0 ? GL_FLOAT : type, normalize ? GL_TRUE : GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
	}
}
