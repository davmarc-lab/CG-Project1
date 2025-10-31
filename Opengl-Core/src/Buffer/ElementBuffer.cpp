#include "../../include/Buffer/ElementBuffer.hpp"

namespace ogl {
	ElementBuffer::~ElementBuffer() {
		if (this->m_id) {
			glDeleteBuffers(1, &this->m_id);
			this->m_id = 0;
		}
	}

	void ElementBuffer::onAttach() {
		if (!this->m_id)
			glGenBuffers(1, &this->m_id);
	}

	void ElementBuffer::onDetach() {
		glDeleteBuffers(1, &this->m_id);
		this->m_id = 0;
	}

	void ElementBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id); }

	void ElementBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
} // namespace ogl
