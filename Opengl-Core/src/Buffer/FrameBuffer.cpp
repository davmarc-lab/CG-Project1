#include "../../include/Buffer/FrameBuffer.hpp"
#include <utility>

namespace ogl {
	FrameBuffer::~FrameBuffer() {
		if (this->m_id) {
			glDeleteFramebuffers(1, &this->m_id);
			this->m_id = 0;
		}
	}

	void FrameBuffer::setRenderBuffer(const RBConfig &config) {
		this->m_rbo = RenderBuffer(config);
		this->m_rbo.onAttach();
	}

	void FrameBuffer::setRenderBuffer(const RenderBuffer &buffer) { this->m_rbo = std::move(buffer); }

	void FrameBuffer::setColorTexture(const Texture &texture) { this->m_texture = std::move(texture); }

	bool FrameBuffer::createFrameBuffer() {
		if (!this->m_rbo.getId())
			return false;
		if (!this->m_texture.getId())
			return false;

		glBindFramebuffer(this->m_config.operation, this->m_id);

		// attach texture
		glFramebufferTexture2D(this->m_config.operation, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_texture.getId(), 0);

		// attach render buffer
		glFramebufferRenderbuffer(this->m_config.operation, this->m_rbo.getAttachmentType(), GL_RENDERBUFFER, this->m_rbo.getId());

		auto res = glCheckFramebufferStatus(this->m_config.operation) == GL_FRAMEBUFFER_COMPLETE;
		glBindFramebuffer(this->m_config.operation, 0);

		return res;
	}

	void FrameBuffer::rescaleFrameBuffer(const unsigned int &width, const unsigned int &height) {
		// calls rescaleTexture()
		this->bind();
		this->m_texture.setSize({width, height});
		this->m_rbo.rescaleRenderBuffer(width, height);
		glFramebufferRenderbuffer(this->m_config.operation, this->m_rbo.getAttachmentType(), GL_RENDERBUFFER, this->m_rbo.getId());
		this->unbind();
	}

	void FrameBuffer::onAttach() {
		if (this->m_id)
			return;

		glGenFramebuffers(1, &this->m_id);
	}

	void FrameBuffer::onDetach() {
		glDeleteFramebuffers(1, &this->m_id);
		this->m_id = 0;
	}

	void FrameBuffer::bind() const {
		glBindFramebuffer(this->m_config.operation, this->m_id);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(.3, .3, .3, 1);
	}

	void FrameBuffer::unbind() const {
		glBindFramebuffer(this->m_config.operation, 0);
	}
} // namespace ogl
