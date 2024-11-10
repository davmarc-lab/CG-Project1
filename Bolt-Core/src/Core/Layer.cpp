#include "../../include/Core/Layer.hpp"

namespace bolt {
	Layer::Layer(std::string name) : m_layerName(std::move(name)) {}

	void LayerStack::addCustomLayer(Shared<Layer> layer) {
		this->m_layers.push_back(layer);
	}
} // namespace Bolt
