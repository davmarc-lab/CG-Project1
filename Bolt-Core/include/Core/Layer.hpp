#pragma once

#include "Core.hpp"
#include "Utils.hpp"

#include "Event.hpp"

#include <mutex>
#include <string>
#include <vector>

namespace bolt {
	/*
	 * Interface for implementing a custom `Layer`.
	 *
	 * All the layers are managed by the `LayerManager`.
	 */
	class Layer {
	protected:
		std::string m_layerName{};
        b8 m_attached = false;

	public:
		explicit Layer(std::string name = "Layer");

		virtual ~Layer() = default;

		/// Initialize the `Layer` elements before entering the game loop.
		virtual void onAttach() {}

		/// Called before the application is closed.
		/// It's used to free all the `Layer` elements.
		virtual void onDetach() {}

		/// ???
		virtual void onEvent(const Event& e) {}

		/// Updates `Layer` elements in the game loop.
		virtual void onUpdate() {}

		/// Called before `Layer::onRender()`, it should contain all the operations before rendering.
		virtual void begin() {}

		/// Render all `Layer` elements.
		virtual void onRender() {}

		/// Called after `Layer::onRender()`.
		virtual void end() {}

        inline b8 isAttached() const { return this->m_attached; }
	};

	class LayerStack {
	private:
		inline static Shared<LayerStack> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		std::vector<Shared<Layer>> m_layers;

		LayerStack() = default;

	public:
		LayerStack(LayerStack& other) = delete;

		void operator=(const LayerStack& other) = delete;

		inline static Shared<LayerStack> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<LayerStack> copy(new LayerStack());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		void addCustomLayer(Shared<Layer> layer);

		inline std::vector<Shared<Layer>> getLayers() const { return this->m_layers; }
	};
} // namespace Bolt
