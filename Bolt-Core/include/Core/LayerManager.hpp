#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "Layer.hpp"
#include "../Graphic/Window.hpp"

namespace bolt {
	/*
	 * This class use a thread safe sinleton to create only one instance of this object.
	 *
	 * A `LayerManager` stores all the different layers.
	 */
	class LayerManager {
	private:
		inline static Shared<LayerManager> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		std::vector<Shared<Layer>> m_layers;

		LayerManager() {}

	public:
		LayerManager(LayerManager& other) = delete;

		void operator=(const LayerManager& other) = delete;

		/*
		 * Retrieves the instance of the `LayerManager` if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `LayerManager` unique pointer.
		 */
		inline static Shared<LayerManager> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<LayerManager> copy(new LayerManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		/*
		 * Store the given `Layer`.
		 * All the operations executed on the layers follow the insert order.
		 *
		 * @param `Shared<Layer>` the layer to be stored.
		 */
		void addLayer(const Shared<Layer>& layer);

		void addLayersFromStack();

		/// ???
		template <class T>
		inline void getLayerFromType() const {
			/* BT_WARN_CORE("{0} method not implemented.", __FILE__); */
		}

		/*
		 * Executes an operation for each layer.
		 *
		 * @param lambda expression.
		 */
		void execute(const std::function<void(Shared<Layer>)>& action);
	};
}
