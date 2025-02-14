#include "../../include/Buffer/UniformBuffer.hpp"

namespace ogl {

UniformBuffer::UniformBuffer(const std::string &variable) : m_variable(std::move(variable)) {}

UniformBuffer::~UniformBuffer() {
    if (this->m_id)
        glDeleteBuffers(1, &this->m_id);
}

void UniformBuffer::onAttach() { glGenBuffers(1, &this->m_id); }

void UniformBuffer::onDetach() { glDeleteBuffers(1, &this->m_id); }

void UniformBuffer::bind() const { glBindBuffer(GL_UNIFORM_BUFFER, this->m_id); }

void UniformBuffer::unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

void UniformBuffer::setup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize) {
    this->bind();
    this->fastSetup(size, bindPoint, offset, rangeSize);
    this->unbind();
}
void UniformBuffer::fastSetup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize) {
    this->m_memAlloc = size;
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);

    if (!(offset && rangeSize))
        glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, this->m_id);
    else
        glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, this->m_id, offset, rangeSize);
}

void UniformBuffer::update(const unsigned int &offset, const unsigned int &size, void *data) {
    this->bind();
    this->fastUpdate(offset, size, data);
    this->unbind();
}
void UniformBuffer::fastUpdate(const unsigned int &offset, const unsigned int &size, void *data) {
    this->m_memAlloc = size;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

} // namespace ogl
