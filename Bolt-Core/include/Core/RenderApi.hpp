#pragma once

#include "Enums.hpp"

#include <cstdlib>
#include <memory>
#include <mutex>

#include "../Graphic/Renderer/Renderer.hpp"
#include "Log.hpp"

namespace bolt {
	class RenderApi {
	private:
		inline static Shared<RenderApi> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		bool m_attached = false;
		Shared<Renderer> m_render = nullptr;

		using RenderApiConfig = config::RenderApiConfig;

		RenderApi() = default;

	public:
		RenderApi(RenderApi& other) = delete;

		void operator=(const RenderApi& other) = delete;

		inline static Shared<RenderApi> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<RenderApi> copy(new RenderApi());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		inline void init(const RenderApiConfig& config = config::render_unknonw) {
			// different renderer types
			if (config & config::render_opengl)
				this->m_render = CreateShared<Renderer>();

			if (this->m_render == nullptr) {
				// BT_CRITICAL_CORE("No renderer attached, closing the application.");
				// close app
				exit(-1);
			}

			this->m_render->onAttach();
			this->m_attached = true;
		}

		Shared<Renderer> getRenderer() const { return this->m_render; }
	};
} // namespace Bolt
