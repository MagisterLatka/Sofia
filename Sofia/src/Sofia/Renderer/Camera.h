#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Camera : public RefCounted
	{
	public:
		SOF_CORE Camera() = default;
		SOF_CORE Camera(const glm::mat4& projectionMatrix) noexcept
			: m_ProjMat(projectionMatrix) {}
		virtual ~Camera() = default;

		SOF_CORE virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;

		SOF_CORE const glm::mat4& GetProjectionMatrix() const noexcept { return m_ProjMat; }
	protected:
		glm::mat4 m_ProjMat = glm::mat4(1.0f);
	};

	class OrthographicCamera : public Camera
	{
		friend class SceneHierarchyPanel;
	public:
		SOF_CORE OrthographicCamera() = default;
		SOF_CORE OrthographicCamera(const glm::mat4& projectionMatrix) noexcept
			: Camera(projectionMatrix) {}
		SOF_CORE OrthographicCamera(uint32_t width, uint32_t height, float nearClip = -1.0f, float farClip = 1.0f);
		SOF_CORE OrthographicCamera(float aspectRatio, float nearClip = -1.0f, float farClip = 1.0f);
		SOF_CORE ~OrthographicCamera() = default;

		SOF_CORE virtual void SetViewportSize(uint32_t width, uint32_t height) override;
		SOF_CORE void SetAspectRatio(float aspectRatio);
		SOF_CORE float GetAspectRatio() const noexcept { return m_AspectRatio; }

		SOF_CORE void SetSize(float size);
		SOF_CORE float GetSize() const noexcept { return m_Size; }
		SOF_CORE float GetWidth() const noexcept { return m_Size * m_AspectRatio; }
		SOF_CORE float GetHeight() const noexcept { return m_Size; }

		SOF_CORE void SetZClips(float nearClip, float farClip);
		SOF_CORE float GetNearClip() const noexcept { return m_NearClip; }
		SOF_CORE float GetFarClip() const noexcept { return m_FarClip; }
	private:
		void Recalc();
	private:
		float m_AspectRatio = 1.0f;
		float m_Size = 1.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;
	};
}
