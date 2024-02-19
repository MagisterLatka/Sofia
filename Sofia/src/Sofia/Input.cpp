#include <pch.h>
#include "Input.h"

#include "Sofia/Application.h"
#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	bool Input::IsKeyPressed(KeyCode key)
	{
		return Application::Get().GetWindow()->GetKeyboard().IsKeyPressed(static_cast<uint8_t>(key));
	}
	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		switch (button)
		{
		case Sofia::MouseCode::Button0:
			return Application::Get().GetWindow()->GetMouse().IsLeftButtonPressed();
		case Sofia::MouseCode::Button1:
			return Application::Get().GetWindow()->GetMouse().IsRightButtonPressed();
		case Sofia::MouseCode::Button2:
		case Sofia::MouseCode::Button3:
		case Sofia::MouseCode::Button4:
		case Sofia::MouseCode::Button5:
		case Sofia::MouseCode::Button6:
		case Sofia::MouseCode::Button7:
			return false;
		}
		return false;
	}
	glm::vec2 Input::GetMousePos()
	{
		return Application::Get().GetWindow()->GetMouse().GetPos();
	}
}
