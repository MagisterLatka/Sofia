#pragma once

#include "Sofia/Core.h"
#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	class Renderer2D
	{
		friend class Renderer;
	public:
		struct Stats
		{
			uint32_t QuadCount = 0u, CircleCount = 0u, DrawCalls = 0u;
		};
	public:
		SOF_CORE static void SetViewProjectionMatrix(const glm::mat4& viewProjMat);

		SOF_CORE void Draw();

		SOF_CORE static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
			const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f, uint32_t entityID = -1);
		SOF_CORE static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
			const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f, uint32_t entityID = -1);
		SOF_CORE static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {},
			float tillingFactor = 1.0f, uint32_t entityID = -1);

		SOF_CORE static void DrawQuads();

		SOF_CORE static void SubmitCircle(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f, float fade = 0.005f, uint32_t entityID = -1);
		SOF_CORE static void SubmitCircle(const glm::vec2& pos, float radius, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f, float fade = 0.005f, uint32_t entityID = -1);
		SOF_CORE static void SubmitCircle(const glm::vec3& pos, float radius, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f, float fade = 0.005f, uint32_t entityID = -1);
		SOF_CORE static void DrawCircles();

		SOF_CORE static Stats GetStats() noexcept;
		SOF_CORE static void ResetStats() noexcept;
	private:
		static void Init();
		static void Shutdown();
	};
}
