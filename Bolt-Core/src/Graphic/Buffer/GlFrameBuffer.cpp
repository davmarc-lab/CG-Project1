#include "../../../include/Graphic/Buffer/FrameBuffer.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

#include <iostream>

namespace bolt {
	void FrameBuffer::onAttach() {
		glGenFramebuffers(1, &this->m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);

		this->m_textureAttach = Texture(texture::Parameters{GL_TEXTURE_2D, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE}, this->m_config.size.x, this->m_config.size.y);
		this->m_textureAttach.onAttach();

		this->m_textureAttach.bind();
		this->m_textureAttach.createTexture2D(NULL);
		this->m_textureAttach.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		this->m_textureAttach.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(this->m_config.operation, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_textureAttach.getId(), 0);
		this->m_textureAttach.unbind();

		this->m_renderBuffer = RenderBuffer(rbo::Config{
			GL_DEPTH24_STENCIL8,
			GL_DEPTH_STENCIL_ATTACHMENT,
			this->m_textureAttach.getWidth(),
			this->m_textureAttach.getHeight()
		});
		this->m_renderBuffer.onAttach();
		glFramebufferRenderbuffer(this->m_config.operation, this->m_renderBuffer.getAttachmentType(), GL_RENDERBUFFER, this->m_renderBuffer.getId());

		if (glCheckFramebufferStatus(this->m_config.operation) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
		}

		glBindFramebuffer(this->m_config.operation, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void FrameBuffer::onDetach() {
		glDeleteFramebuffers(1, &this->m_id);
	}

	void FrameBuffer::bind() const {
		glBindFramebuffer(this->m_config.operation, this->m_id);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::unbind() const {
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(this->m_config.operation, 0);
	}

	void FrameBuffer::rescaleFrameBuffer(const u16& width, const u16& height) {
		if (width != 0 && height != 0) {
			this->m_config.size = {width, height};
			glBindFramebuffer(this->m_config.operation, this->m_id);
			this->m_textureAttach.rescaleTexture(width, height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_textureAttach.getId(), 0);

			this->m_renderBuffer.rescaleRenderBuffer(width, height);
			glFramebufferRenderbuffer(this->m_config.operation, this->m_renderBuffer.getAttachmentType(), GL_RENDERBUFFER, this->m_renderBuffer.getId());
		}
	}
} // namespace bolt
