#pragma once

#include "Sofia/Layer.h"

namespace Sofia {

	class LayerStack
	{
		friend class Application;
	public:
		SOF_CORE LayerStack() = default;
		SOF_CORE ~LayerStack();

		SOF_CORE Layer* PushLayer(Layer* layer);
		SOF_CORE Layer* PopLayer(Layer* layer);
		SOF_CORE Layer* PushOverlay(Layer* overlay);
		SOF_CORE Layer* PopOverlay(Layer* overlay);

		SOF_CORE std::vector<Layer*>::iterator begin() noexcept { return m_Layers.begin(); }
		SOF_CORE std::vector<Layer*>::iterator end() noexcept { return m_Layers.end(); }
		SOF_CORE std::vector<Layer*>::const_iterator begin() const noexcept { return m_Layers.begin(); }
		SOF_CORE std::vector<Layer*>::const_iterator end() const noexcept { return m_Layers.end(); }

		SOF_CORE std::vector<Layer*>::reverse_iterator rbegin() noexcept { return m_Layers.rbegin(); }
		SOF_CORE std::vector<Layer*>::reverse_iterator rend() noexcept { return m_Layers.rend(); }
		SOF_CORE std::vector<Layer*>::const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
		SOF_CORE std::vector<Layer*>::const_reverse_iterator rend() const noexcept { return m_Layers.rend(); }
	private:
		void Init();
		void Shutdown();
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0u;
		bool m_IsInitialized = false;
	};
}
