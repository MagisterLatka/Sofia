#include <pch.h>
#include "LayerStack.h"

namespace Sofia {

	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	Layer* LayerStack::PushLayer(Layer* layer) noexcept
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		++m_LayerInsertIndex;
		layer->OnAttach();
		return layer;
	}
	Layer* LayerStack::PushOverlay(Layer* overlay) noexcept
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
		return overlay;
	}
	Layer* LayerStack::PopLayer(Layer* layer) noexcept
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			--m_LayerInsertIndex;
		}
		return layer;
	}
	Layer* LayerStack::PopOverlay(Layer* overlay) noexcept
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
		return overlay;
	}
}
