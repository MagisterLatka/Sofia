#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	class Renderer2D
	{
		friend class Renderer;
	public:
		struct Stats
		{
			uint32_t QuadCount = 0u;
			uint32_t CircleCount = 0u;
			uint32_t DrawCalls = 0u;
		};
	public:
		static void SetViewProjectionMatrix(const glm::mat4& viewProjMat);

		static void Draw();

		static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
			const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f, uint32_t entityID = -1);
		static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
			const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f, uint32_t entityID = -1);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const Ref<Texture2D>& texture = {},
			float tillingFactor = 1.0f, uint32_t entityID = -1);
		static void DrawQuads();

		static void SubmitCircle(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f, float fade = 0.005f, uint32_t entityID = -1);
		static void DrawCircles();

		static Stats GetStats() noexcept;
		static void ResetStats() noexcept;
	private:
		static void Init();
		static void Shutdown();
	};
}
