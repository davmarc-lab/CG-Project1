#include "../../../include/Graphic/Texture/Texture.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

#include <cassert>

namespace bolt {
	void Texture::onAttach() {
		glGenTextures(1, &this->m_id);
		this->m_generated = true;
	}

	void Texture::onDetach() {
		glDeleteTextures(1, &this->m_id);
		this->m_generated = false;
	}

	void Texture::bind() {
		// always activate the first texture unit
		assert(this->m_ready);
		// glActiveTexture(GL_TEXTURE0);
		glBindTexture(this->m_parameters.target, this->m_id);
	}

	void Texture::unbind() {}

	void Texture::setTexParameter(u32 pname, i32 pval) {
		glTexParameteri(this->m_parameters.target, pname, pval);
	}

	void Texture::createTexture2D(void* data) {
		this->bind();
		glTexImage2D(this->m_parameters.target, this->m_parameters.level, static_cast<i32>(this->m_parameters.internalFormat), this->m_width, this->m_height, this->m_parameters.border,
			this->m_parameters.format, this->m_parameters.dataType, data);
		this->m_created = true;
	}

	void Texture::fastCreateTexture2D(void* data) {
		glTexImage2D(this->m_parameters.target, this->m_parameters.level, static_cast<i32>(this->m_parameters.internalFormat), this->m_width, this->m_height, this->m_parameters.border,
			this->m_parameters.format, this->m_parameters.dataType, data);
		this->m_created = true;
	}

	void Texture::rescaleTexture(const u16& width, const u16& height) {
		this->m_width = width;
		this->m_height = height;
		this->bind();
		this->createTexture2D(NULL);
		this->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		this->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::generateMipmap() {
		if (this->m_created)
			glGenerateMipmap(this->m_parameters.target);
	}
} // namespace Bolt
