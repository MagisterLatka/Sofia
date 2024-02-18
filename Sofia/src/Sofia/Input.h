#pragma once

#include "Sofia/Core.h"
#include "Sofia/KeyCodes.h"
#include "Sofia/MouseCodes.h"

namespace Sofia {

	class SOF_CORE Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePos();
	};
}
