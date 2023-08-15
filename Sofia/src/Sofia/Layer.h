#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"
#include "Sofia/Events/Event.h"

namespace Sofia {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") noexcept
			: m_Name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& e) {}
	protected:
		std::string m_Name;
	};
}
