#include <cassert>

#include "../../../include/Graphic/Buffer/RenderBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

namespace bolt {
	void RenderBuffer::onAttach() {
		assert(this->m_ready);

		glGenRenderbuffers(1, &this->m_id);
		this->bind();
		glRenderbufferStorage(GL_RENDERBUFFER, this->m_config.format, this->m_config.width, this->m_config.height);
		this->unbind();
	}

	void RenderBuffer::onDetach() {
		glDeleteRenderbuffers(1, &this->m_id);
	}

	void RenderBuffer::bind() const {
		glBindRenderbuffer(GL_RENDERBUFFER, this->m_id);
	}

	void RenderBuffer::unbind() const {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void RenderBuffer::rescaleRenderBuffer(const u16& width, const u16& height) {
		this->m_config.width = width;
		this->m_config.height = height;
		this->bind();
		glRenderbufferStorage(GL_RENDERBUFFER, this->m_config.format, this->m_config.width, this->m_config.height);
		this->unbind();
	}

	u32 RenderBuffer::getAttachmentType() const { return this->m_config.attachment; }
} // namespace bolt
