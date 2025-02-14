#include "../../include/Buffer/RenderBuffer.hpp"

namespace ogl {
RenderBuffer::~RenderBuffer() {
    if (this->m_id)
        glDeleteRenderbuffers(1, &this->m_id);
}

void RenderBuffer::bind() const { glBindRenderbuffer(GL_RENDERBUFFER, this->m_id); }

void RenderBuffer::unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void RenderBuffer::onAttach() {
    if (this->m_id)
        return;

    glGenRenderbuffers(1, &this->m_id);
    this->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, this->m_config.format, this->m_config.width, this->m_config.height);
}

void RenderBuffer::onDetach() { glDeleteRenderbuffers(1, &this->m_id); }

void RenderBuffer::rescaleRenderBuffer(const unsigned int &width, const unsigned int &height) {
    this->m_config.width = width;
    this->m_config.height = height;
    this->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, this->m_config.format, this->m_config.width, this->m_config.height);
}
} // namespace ogl
