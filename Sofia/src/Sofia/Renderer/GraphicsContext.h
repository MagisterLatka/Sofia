#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class GraphicsContext : public RefCounted
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;

		static Ref<GraphicsContext> Create();
	};
}
