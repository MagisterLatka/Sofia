#include <pch.h>
#include "EditorCamera.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Input.h"

namespace Sofia {

	static glm::vec3 GetUpDirection(glm::quat quat) noexcept
	{
		return glm::rotate(quat, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	static glm::vec3 GetRightDirection(glm::quat quat) noexcept
	{
		return glm::rotate(quat, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	static glm::vec3 GetForwardDirection(glm::quat quat) noexcept
	{
		return glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f));
	}
	static glm::vec2 GetPanSpeed(uint32_t viewportWidth, uint32_t viewportHeight) noexcept
	{
		float x = glm::min((float)viewportWidth / 1000.0f, 2.4f);
		float xSpeed = 0.0366f * x * x - 0.1778f * x + 0.3021f;
		float y = glm::min((float)viewportHeight/ 1000.0f, 2.4f);
		float ySpeed = 0.0366f * y * y - 0.1778f * y + 0.3021f;
		return { xSpeed, ySpeed };
	}
	static float GetZoomSpeed(float distance) noexcept
	{
		float d = glm::max(distance * 0.2f, 0.0f);
		return glm::min(glm::pow(d, 1.2f), 10.0f);
	}

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) noexcept
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateProj();
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(KeyCode::LeftAlt))
		{
			glm::vec2 delta = (Input::GetMousePos() - m_MousePos) * 0.003f;
			m_MousePos = Input::GetMousePos();
			glm::quat orientation = GetOrientation();

			if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
			{
				static constexpr float rotateSpeed = 0.8f;
				m_Yaw += (GetUpDirection(orientation).y < 0.0f ? -1.0f : 1.0f) * delta.x * rotateSpeed;
				m_Pitch += delta.y * rotateSpeed;
			}
			else if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
			{
				m_Distance = glm::max(1.0f, m_Distance - delta.y * GetZoomSpeed(m_Distance));
			}
			else if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
			{
				glm::vec2 panSpeed = GetPanSpeed(m_ViewportWidth, m_ViewportHeight);
				m_FocalPoint += -GetRightDirection(orientation) * delta.x * panSpeed.x * m_Distance;
				m_FocalPoint += GetUpDirection(orientation) * delta.y * panSpeed.y * m_Distance;
			}
		}
		UpdateView();
	}
	void EditorCamera::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SOF_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}
	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_Distance = glm::max(1.0f, m_Distance - e.GetYOffset() * 0.1f * GetZoomSpeed(m_Distance));
		return false;
	}

	void EditorCamera::UpdateProj()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return;
		case RendererAPI::API::OpenGL:
			m_ProjMat = glm::perspectiveRH_NO(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
			return;
		case RendererAPI::API::Vulkan:
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:
			m_ProjMat = glm::perspectiveRH_ZO(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
			return;
		}
		SOF_CORE_THROW_INFO("Unknown API");
	}
	void EditorCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();
		m_Position = m_FocalPoint - GetForwardDirection(orientation) * m_Distance;
		m_ViewMat = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation));
	}

	glm::quat EditorCamera::GetOrientation() noexcept
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height) noexcept
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		UpdateProj();
	}
}
