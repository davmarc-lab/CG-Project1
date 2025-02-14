#include "../../include/Buffer/VertexBuffer.hpp"

namespace ogl {
VertexBuffer::~VertexBuffer() {
    if (this->m_id)
        glDeleteBuffers(1, &this->m_id);
}

void VertexBuffer::onAttach() {
    if (!this->m_id)
        glGenBuffers(1, &this->m_id);
}

void VertexBuffer::onDetach() { glDeleteBuffers(1, &this->m_id); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, this->m_id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

} // namespace ogl
