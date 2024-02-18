#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class GraphicsContext : public RefCounted
	{
	public:
		SOF_CORE virtual ~GraphicsContext() = default;

		SOF_CORE virtual void Init() = 0;

		SOF_CORE static Ref<GraphicsContext> Create();
	};
}
