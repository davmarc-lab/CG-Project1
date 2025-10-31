#include "../../include/Buffer/VertexBuffer.hpp"

namespace ogl {
	VertexBuffer::~VertexBuffer() {
		if (this->m_id) {
			glDeleteBuffers(1, &this->m_id);
			this->m_id = 0;
		}
	}

	void VertexBuffer::onAttach() {
		glGenBuffers(1, &this->m_id);
	}

	void VertexBuffer::onDetach() {
		glDeleteBuffers(1, &this->m_id);
		this->m_id = 0;
	}

	void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, this->m_id); }

	void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

} // namespace ogl
