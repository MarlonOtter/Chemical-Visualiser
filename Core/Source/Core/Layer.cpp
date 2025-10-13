#include "Layer.h"

#include "Application.h"

namespace Core
{
	//? Implement later with thread safety
	void Layer::QueueTransition(std::unique_ptr<Layer> toLayer) {
		auto& LayerStack = Application::Get().m_LayerStack;
		for (auto& layer : LayerStack) {
			if (layer.get() == this) {
				layer = std::move(toLayer);
				return;
			}
		}
	}
}