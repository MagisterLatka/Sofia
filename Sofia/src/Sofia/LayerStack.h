#pragma once

#include "Sofia/Layer.h"

namespace Sofia {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		Layer* PushLayer(Layer* layer) noexcept;
		Layer* PopLayer(Layer* layer) noexcept;
		Layer* PushOverlay(Layer* overlay) noexcept;
		Layer* PopOverlay(Layer* overlay) noexcept;

		std::vector<Layer*>::iterator begin() noexcept { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() noexcept { return m_Layers.end(); }
		std::vector<Layer*>::const_iterator begin() const noexcept { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const noexcept { return m_Layers.end(); }

		std::vector<Layer*>::reverse_iterator rbegin() noexcept { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() noexcept { return m_Layers.rend(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const noexcept { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0u;
	};
}
