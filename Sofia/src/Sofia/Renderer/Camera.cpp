#include <pch.h>
#include "Camera.h"

#include "Sofia/Renderer/RendererAPI.h"

namespace Sofia {

	OrthographicCamera::OrthographicCamera(uint32_t width, uint32_t height, float nearClip, float farClip)
		: m_AspectRatio((float)width / (float)height), m_NearClip(nearClip), m_FarClip(farClip)
	{
		Recalc();
	}
	OrthographicCamera::OrthographicCamera(float aspectRatio, float nearClip, float farClip)
		: m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		Recalc();
	}

	void OrthographicCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		Recalc();
	}
	void OrthographicCamera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		Recalc();
	}
	void OrthographicCamera::SetSize(float size)
	{
		m_Size = size;
		Recalc();
	}
	void OrthographicCamera::SetZClips(float nearClip, float farClip)
	{
		m_NearClip = nearClip;
		m_FarClip = farClip;
		Recalc();
	}
	void OrthographicCamera::Recalc()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return;
		case RendererAPI::API::OpenGL:
			m_ProjMat = glm::orthoRH_NO(-m_AspectRatio * m_Size * 0.5f, m_AspectRatio * m_Size * 0.5f, -m_Size * 0.5f, m_Size * 0.5f, m_NearClip, m_FarClip);
			return;
		case RendererAPI::API::Vulkan:
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:
			m_ProjMat = glm::orthoRH_ZO(-m_AspectRatio * m_Size * 0.5f, m_AspectRatio * m_Size * 0.5f, -m_Size * 0.5f, m_Size * 0.5f, m_NearClip, m_FarClip);
			return;
		}
		SOF_CORE_THROW_INFO("Unknown API");
	}
}
