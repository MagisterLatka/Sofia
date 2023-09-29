#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Camera : public RefCounted
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) noexcept : m_Projection(projection) {}
		virtual ~Camera() = default;

		virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;

		const glm::mat4& GetProjection() const noexcept { return m_Projection; }
		static glm::mat4 GetCameraViewMatrix(const glm::mat4& cameraTransform);
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

	class OrthographicCamera : public Camera
	{
		friend class SceneHierarchyPanel;
	public:
		OrthographicCamera() = default;
		OrthographicCamera(const glm::mat4& projection) noexcept : Camera(projection) {}
		OrthographicCamera(float width, float height, float nearClip = -1.0f, float farClip = 1.0f);
		OrthographicCamera(float aspectRatio, float nearClip = -1.0f, float farClip = 1.0f);
		~OrthographicCamera() = default;

		virtual void SetViewportSize(uint32_t width, uint32_t height) override;
		void SetAspectRatio(float aspectRatio);
		float GetAspectRatio() const noexcept { return m_AspectRatio; }

		void SetSize(float size);
		float GetSize() const noexcept { return m_Size; }
		float GetWidth() const noexcept { return m_AspectRatio * m_Size; }
		float GetHeight() const noexcept { return m_Size; }

		void SetZClips(float nearClip, float farClip);
		float GetNearClip() const noexcept { return m_NearClip; }
		float GetFarClip() const noexcept { return m_FarClip; }
	private:
		void Recalc();
	private:
		float m_AspectRatio = 1.0f;
		float m_Size = 1.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;
	};
}
