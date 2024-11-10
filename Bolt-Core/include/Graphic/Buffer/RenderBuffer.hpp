#pragma once

#include "../../Core/Utils.hpp"

namespace bolt {
	namespace rbo {
		struct Config {
			u32 format;
			u32 attachment;
			u32 width;
			u32 height;
		};
	}

	class RenderBuffer {
	private:
		u32 m_id = 0;
		rbo::Config m_config{};
		b8 m_ready = false;

	public:
		RenderBuffer() = default;

		explicit RenderBuffer(const rbo::Config config) : m_config(config) { this->m_ready = true; }

		~RenderBuffer() = default;

		void onAttach();

		void onDetach();

		void bind() const;

		void unbind() const;

		void rescaleRenderBuffer(const u16& width, const u16& height);

		u32 getAttachmentType() const;

		const u32& getId() const { return this->m_id; }
	};
} // namespace Bolt
