#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"
#include "Sofia/Events/Event.h"

namespace Sofia {

	class  Layer
	{
	public:
		SOF_CORE Layer(const std::string& name = "Layer") noexcept
			: m_Name(name) {}
		SOF_CORE virtual ~Layer() = default;

		SOF_CORE virtual void OnAttach() {}
		SOF_CORE virtual void OnDetach() {}
		SOF_CORE virtual void OnUpdate(Timestep ts) {}
		SOF_CORE virtual void OnUIRender() {}
		SOF_CORE virtual void OnEvent(Event& e) {}
	protected:
		std::string m_Name;
	};
}
