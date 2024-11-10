#pragma once

#include "../../Core/Structs.hpp"
#include "../../Core/Utils.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

#include "../Texture/Texture.hpp"

#include "RenderBuffer.hpp"

namespace bolt {
	namespace fbo {
		enum Operation : u32 {
			FB_READ    = GL_READ_FRAMEBUFFER,
			FB_WRITE   = GL_DRAW_FRAMEBUFFER,
			FB_DEFAULT = GL_FRAMEBUFFER
		};

		struct Config {
			Pair<u16> size = {0, 0};
			Operation operation = FB_DEFAULT;
		} inline defaultConfig;
	} // namespace fbo

	class FrameBuffer {
	protected:
		u32 m_id = 0;

		using Config = fbo::Config;

		Config m_config{};
		Texture m_textureAttach{};
		RenderBuffer m_renderBuffer{};

	public:
		FrameBuffer() = default;

		FrameBuffer(Config config) : m_config(config) {}

		~FrameBuffer() = default;

		void onAttach();

		void onDetach();

		void bind() const;

		void unbind() const;

		void rescaleFrameBuffer(const u16& width, const u16& height);

		const u32& getId() const { return this->m_id; }

		u16 getTextureId() const { return this->m_textureAttach.getId(); }
	};
} // namespace bolt
