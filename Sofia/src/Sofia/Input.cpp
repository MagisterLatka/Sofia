#include <pch.h>
#include "Input.h"

#include "Sofia/Application.h"
#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	bool Input::IsKeyPressed(KeyCode key)
	{
		uint8_t keycode = 0;
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); break;
		case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); break;
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); break;
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:	keycode = GetVKCode(key);
		}
		return Application::Get().GetWindow()->GetKeyboard().IsKeyPressed(keycode);
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
