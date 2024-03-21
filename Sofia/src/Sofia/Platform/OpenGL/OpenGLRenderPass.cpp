#include <pch.h>
#include "OpenGLRenderPass.h"

#include "Sofia/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Sofia {

	OpenGLRenderPass::OpenGLRenderPass()
	{
		Create();
	}
	void OpenGLRenderPass::Create()
	{
		Ref<OpenGLRenderPass> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateFramebuffers(1, &instance->m_ID);
		});
	}
	OpenGLRenderPass::~OpenGLRenderPass()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLRenderPass::SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget)
	{
		SOF_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
		m_RenderTargets[index] = renderTarget.As<OpenGLRenderTarget>();
		if (m_RenderTargets[index])
		{
			SOF_CORE_ASSERT(!((uint32_t)m_RenderTargets[index]->m_Format & 0x30u), "Render target cannot have depth or depth stencil format");

			if (m_Width == 0 || m_Height == 0)
			{
				m_Width = m_RenderTargets[index]->m_Width;
				m_Height = m_RenderTargets[index]->m_Height;
			}
		}
	}
	void OpenGLRenderPass::SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget)
	{
		m_DepthStencilTarget = depthStencilTarget.As<OpenGLRenderTarget>();
		if (m_DepthStencilTarget)
			SOF_CORE_ASSERT((uint32_t)m_DepthStencilTarget->m_Format & 0x30u, "Depth stencil target must have depth or depth stencil format");
	}
	void OpenGLRenderPass::Bind() const noexcept
	{
		Ref<const OpenGLRenderPass> instance = this;
		Renderer::Submit([instance]()
		{
			for (int i = 0; i < 8; ++i)
			{
				if (instance->m_RenderTargets[i])
					glNamedFramebufferTexture(instance->m_ID, GL_COLOR_ATTACHMENT0 + i, instance->m_RenderTargets[i]->m_ID, 0);
			}

			if (instance->m_DepthStencilTarget)
			{
				if (instance->m_DepthStencilTarget->m_Format == RenderTargetFormat::Depth32F)
					glNamedFramebufferTexture(instance->m_ID, GL_DEPTH_ATTACHMENT, instance->m_DepthStencilTarget->m_ID, 0);
				else
					glNamedFramebufferTexture(instance->m_ID, GL_DEPTH_STENCIL_ATTACHMENT, instance->m_DepthStencilTarget->m_ID, 0);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, instance->m_ID);
			glViewport(0, 0, instance->m_Width, instance->m_Height);
		});
	}
	void OpenGLRenderPass::Clear(const glm::vec4& clearVal, float depth, uint8_t stencil) noexcept
	{
		Ref<OpenGLRenderPass> instance = this;
		Renderer::Submit([instance, clearVal, depth, stencil]()
		{
			for (uint32_t i = 0; i < 8; ++i)
			{
				if (instance->m_RenderTargets[i])
				{
					glClearNamedFramebufferfv(instance->m_ID, GL_COLOR, 0, glm::value_ptr(clearVal));
				}
			}
			if (instance->m_DepthStencilTarget)
			{
				if (instance->m_DepthStencilTarget->m_Format == RenderTargetFormat::Depth32F)
					glClearNamedFramebufferfv(instance->m_ID, GL_DEPTH, 0, &depth);
				else
					glClearNamedFramebufferfi(instance->m_ID, GL_DEPTH_STENCIL, 0, depth, stencil);
			}
		});
	}
	void OpenGLRenderPass::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		for (uint32_t i = 0; i < 8; ++i)
		{
			if (m_RenderTargets[i])
				m_RenderTargets[i]->Resize(m_Width, m_Height);
		}
		if (m_DepthStencilTarget)
			m_DepthStencilTarget->Resize(m_Width, m_Height);
	}
}
