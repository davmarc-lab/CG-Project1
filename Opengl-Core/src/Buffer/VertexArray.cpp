#include "../../include/Buffer/VertexArray.hpp"

namespace ogl {
VertexArray::~VertexArray() {
    if (this->m_id)
        glDeleteVertexArrays(1, &this->m_id);
}

void VertexArray::bind() const { glBindVertexArray(this->m_id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::onAttach() { glGenVertexArrays(1, &this->m_id); }

void VertexArray::onDetach() { glDeleteVertexArrays(1, &this->m_id); }

void VertexArray::linkAttribFast(const unsigned int &layout, const int &size, const unsigned int &type, const bool &normalize, const int &stride,
                                 void *offset) const {
    glVertexAttribPointer(layout, size, type, normalize ? GL_TRUE : GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}
} // namespace ogl
