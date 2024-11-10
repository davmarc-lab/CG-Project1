#pragma once

#include "../../Core/Utils.hpp"

namespace bolt {
	namespace texture {
		struct Parameters {
			u32 target;
			i32 level = 0;
			u32 internalFormat;
			i32 border;
			u32 format;
			u32 dataType;
		};
	}

	class Texture {
	protected:
		u32 m_id = 0;
		u32 m_width = 0;
		u32 m_height = 0;
		texture::Parameters m_parameters{};

		b8 m_generated = false;
		b8 m_ready = false;
		b8 m_created = false;

	public:
		Texture() = default;

		Texture(const texture::Parameters& params, const u32 width, const u32 height) : m_width(width), m_height(height), m_parameters(params) { this->m_ready = true; }

		~Texture() = default;

		void onAttach();

		void onDetach();

		void bind();

		void unbind();

		void setTexParameter(u32 pname, i32 pval);

		void createTexture2D(void* data);

		void fastCreateTexture2D(void* data);

		void rescaleTexture(const u16& width, const u16& height);

		void generateMipmap();

		b8 isTextureIdGenerated() const { return this->m_generated; }

		u32 getId() const { return this->m_id; }

		b8 isTextureCreated() const { return this->m_created; }

		inline u32 getWidth() const { return this->m_width; }

		inline u32 getHeight() const { return this->m_height; }
	};
} // namespace Bolt
