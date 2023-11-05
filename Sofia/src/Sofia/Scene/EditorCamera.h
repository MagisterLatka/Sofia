#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/MouseEvents.h"
#include "Sofia/Timestep.h"

namespace Sofia {

	class EditorCamera : public RefCounted
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov /*in radians*/, float aspectRatio, float nearClip = 0.1f, float farClip = 100.0f) noexcept;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetViewportSize(uint32_t width, uint32_t height) noexcept;
		float GetDistance() const noexcept { return m_Distance; }
		float& GetDistance() noexcept { return m_Distance; }
		const glm::vec3& GetPosition() const noexcept { return m_Position; }
		glm::vec3& GetPosition() noexcept { return m_Position; }
		float GetPitch() const noexcept { return m_Pitch; }
		float GetYaw() const noexcept { return m_Yaw; }

		const glm::mat4& GetProjectionMatrix() const noexcept { return m_ProjMat; }
		const glm::mat4& GetViewMatrix() const noexcept { return m_ViewMat; }
		glm::mat4 GetProjectionViewMatrix() const noexcept { return m_ProjMat * m_ViewMat; }
	private:
		void UpdateProj();
		void UpdateView();
		glm::quat GetOrientation() noexcept;

		bool OnMouseScroll(MouseScrolledEvent& e);
	private:
		glm::mat4 m_ProjMat = glm::mat4(1.0f), m_ViewMat = glm::mat4(1.0f);
		float m_FOV = glm::half_pi<float>(), m_AspectRatio = 16.0f / 9.0f, m_NearClip = 0.1f, m_FarClip = 100.0f;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_MousePos = { 0.0f, 0.0f };
		float m_Distance = 5.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
		uint32_t m_ViewportWidth = 1600u, m_ViewportHeight = 900u;
	};
}
