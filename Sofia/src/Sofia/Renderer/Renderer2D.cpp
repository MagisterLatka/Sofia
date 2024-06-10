#include <pch.h>
#include "Renderer2D.h"

#include "Sofia/Renderer/InputLayout.h"
#include "Sofia/Renderer/Renderer.h"
#include "Sofia/Renderer/RenderCommand.h"

namespace Sofia {

	struct QuadVertexData
	{
		glm::vec4 pos;
		glm::vec4 color;
		glm::vec2 uv;
		int tid;
		float tillingFactor;
		uint32_t id;
		glm::ivec3 padding;
	};
	struct CircleVertexData
	{
		glm::vec4 pos;
		glm::vec4 color;
		float thickness;
		float fade;
		uint32_t id;
		uint32_t padding;
	};

	static constexpr uint32_t c_MaxQuads = 10000u;
	static constexpr uint32_t c_QuadBufferSize = c_MaxQuads * 4 * sizeof(QuadVertexData);
	static constexpr uint32_t c_MaxCircles = 10000u;
	static constexpr uint32_t c_CircleBufferSize = c_MaxCircles * 4 * sizeof(CircleVertexData);
	static constexpr uint32_t c_MaxTextures = 16u;

	struct Renderer2DData
	{
		QuadVertexData* quadVertexData = nullptr;
		QuadVertexData* quadInsert = nullptr;
		uint32_t quadCount = 0u;
		Ref<InputLayout> quadInputLayout;
		std::array<Ref<Texture2D>, c_MaxTextures> quadTextures;
		uint32_t quadTextureIndex = 1u;
		Ref<Shader> quadShader;

		CircleVertexData* circleVertexData = nullptr;
		CircleVertexData* circleInsert = nullptr;
		uint32_t circleCount = 0u;
		Ref<InputLayout> circleInputLayout;
		Ref<Shader> circleShader;

		Ref<ConstantBuffer> viewProj;

		Renderer2D::Stats stats;
	};
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.quadVertexData = new QuadVertexData[c_MaxQuads * 4];
		s_Data.quadInsert = s_Data.quadVertexData;

		constexpr glm::vec2 uv[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		for (uint32_t i = 0; i < c_MaxQuads * 4; i += 4)
		{
			s_Data.quadVertexData[i + 0].uv = uv[0];
			s_Data.quadVertexData[i + 1].uv = uv[1];
			s_Data.quadVertexData[i + 2].uv = uv[2];
			s_Data.quadVertexData[i + 3].uv = uv[3];
		}

		uint32_t* indices = new uint32_t[c_MaxQuads * 6];
		uint32_t index = 0;
		for (uint32_t i = 0; i < c_MaxQuads * 6; i += 6)
		{
			indices[i + 0] = index + 0;
			indices[i + 1] = index + 1;
			indices[i + 2] = index + 2;

			indices[i + 3] = index + 2;
			indices[i + 4] = index + 3;
			indices[i + 5] = index + 0;

			index += 4;
		}

		s_Data.quadShader = Renderer::GetShaderLibrary().Get("quadShader");
		UniformBuffer<sizeof(glm::mat4), 1> uniformBuffer;
		uniformBuffer.Push("u_ViewProjMat", glm::mat4(1.0f));
		s_Data.viewProj = ConstantBuffer::Create(BufferShaderBinding::Vertex, uniformBuffer);

		BufferLayout quadLayout = {
			{ "Position", BufferLayoutElementDataType::Float4 },
			{ "Color", BufferLayoutElementDataType::Float4 },
			{ "UV", BufferLayoutElementDataType::Float2 },
			{ "TID", BufferLayoutElementDataType::Int },
			{ "TillingFactor", BufferLayoutElementDataType::Float },
			{ "EntityID", BufferLayoutElementDataType::UInt4 }
		};
		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(quadLayout, s_Data.quadVertexData, c_QuadBufferSize);
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indices, c_MaxQuads * 6 * sizeof(uint32_t));
		s_Data.quadInputLayout = InputLayout::Create({ quadVBO }, s_Data.quadShader, quadIBO);

		uint32_t texData = 0xffffffffu;
		s_Data.quadTextures[0] = Texture2D::Create(1u, 1u, &texData);



		s_Data.circleVertexData = new CircleVertexData[c_MaxCircles * 4];
		s_Data.circleInsert = s_Data.circleVertexData;

		s_Data.circleShader = Renderer::GetShaderLibrary().Get("circleShader");
		BufferLayout circleLayout = {
			{ "Position", BufferLayoutElementDataType::Float4 },
			{ "Color", BufferLayoutElementDataType::Float4 },
			{ "Thickness", BufferLayoutElementDataType::Float },
			{ "Fade", BufferLayoutElementDataType::Float },
			{ "EntityID", BufferLayoutElementDataType::UInt2 }
		};
		Ref<VertexBuffer> circleVBO = VertexBuffer::Create(circleLayout, nullptr, c_CircleBufferSize, BufferUsage::Dynamic);
		Ref<IndexBuffer> circleIBO = IndexBuffer::Create(indices, c_MaxQuads * 6 * sizeof(uint32_t));
		s_Data.circleInputLayout = InputLayout::Create({ circleVBO }, s_Data.circleShader, circleIBO);
		delete[] indices;
	}
	void Renderer2D::Shutdown()
	{
		for (uint32_t i = 0; i < c_MaxTextures; ++i)
			s_Data.quadTextures[i].Reset();
		s_Data.quadInputLayout.Reset();
		s_Data.viewProj.Reset();
		s_Data.quadShader.Reset();
		delete[] s_Data.quadVertexData;

		s_Data.circleInputLayout.Reset();
		s_Data.circleShader.Reset();
		delete[] s_Data.circleVertexData;
	}

	void Renderer2D::SetViewProjectionMatrix(const glm::mat4& viewProjMat)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			SOF_CORE_THROW_INFO("None API is not supported");
			break;
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::Vulkan:
			s_Data.viewProj->SetData((void*)glm::value_ptr(viewProjMat), sizeof(glm::mat4));
			break;
		case RendererAPI::API::DX11:
		case RendererAPI::API::DX12:
			glm::mat4 transposed = glm::transpose(viewProjMat);
			s_Data.viewProj->SetData((void*)glm::value_ptr(transposed), sizeof(glm::mat4));
			break;
		}
	}
	void Renderer2D::Draw()
	{
		DrawQuads();
		DrawCircles();
	}
	void Renderer2D::SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tillingFactor, uint32_t entityID)
	{
		SubmitQuad(glm::vec3(pos, 0.0f), size, rotation, color, texture, tillingFactor, entityID);
	}
	void Renderer2D::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tillingFactor, uint32_t entityID)
	{
		if (s_Data.quadCount >= c_MaxQuads)
			DrawQuads();

		uint32_t tid = 0u;
		if (texture)
		{
			for (uint32_t i = 1u; i < s_Data.quadTextureIndex; ++i)
			{
				if (s_Data.quadTextures[i] == texture)
				{
					tid = i;
					break;
				}
			}
			if (tid == 0u)
			{
				if (s_Data.quadTextureIndex >= c_MaxTextures)
					DrawQuads();

				tid = s_Data.quadTextureIndex++;
				s_Data.quadTextures[tid] = texture;
			}
		}

		static constexpr glm::vec3 position[4] = {
			{ -0.5f,  0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f }
		};

		glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		for (int i = 0; i < 4; ++i)
		{
			s_Data.quadInsert->pos = glm::vec4(rotation != 0.0f ? rotate * (position[i] * glm::vec3(size, 1.0f)) + pos : position[i] * glm::vec3(size, 1.0f) + pos, 1.0f);
			s_Data.quadInsert->color = color;
			s_Data.quadInsert->tid = tid;
			s_Data.quadInsert->tillingFactor = tillingFactor;
			s_Data.quadInsert->id = entityID;
			++s_Data.quadInsert;
		}

		++s_Data.quadCount;
		++s_Data.stats.QuadCount;
	}
	void Renderer2D::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tillingFactor, uint32_t entityID)
	{
		if (s_Data.quadCount >= c_MaxQuads)
			DrawQuads();

		uint32_t tid = 0u;
		if (texture)
		{
			for (uint32_t i = 1u; i < s_Data.quadTextureIndex; ++i)
			{
				if (s_Data.quadTextures[i] == texture)
				{
					tid = i;
					break;
				}
			}
			if (tid == 0u)
			{
				if (s_Data.quadTextureIndex >= c_MaxTextures)
					DrawQuads();

				tid = s_Data.quadTextureIndex++;
				s_Data.quadTextures[tid] = texture;
			}
		}

		static constexpr glm::vec4 position[4] = {
			{ -0.5f,  0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.0f, 1.0f }
		};

		for (int i = 0; i < 4; ++i)
		{
			s_Data.quadInsert->pos = transform * position[i];
			s_Data.quadInsert->color = color;
			s_Data.quadInsert->tid = tid;
			s_Data.quadInsert->tillingFactor = tillingFactor;
			s_Data.quadInsert->id = entityID;
			++s_Data.quadInsert;
		}

		++s_Data.quadCount;
		++s_Data.stats.QuadCount;
	}
	void Renderer2D::DrawQuads()
	{
		if (s_Data.quadCount == 0u)
			return;

		s_Data.quadInputLayout->GetVertexBuffer()->SetData();
		s_Data.quadShader->Bind();
		s_Data.viewProj->Bind(0);

		for (uint32_t i = 0; i < s_Data.quadTextureIndex; ++i)
		{
			if (s_Data.quadTextures[i])
				s_Data.quadTextures[i]->Bind(i);
		}

		s_Data.quadInputLayout->Bind();
		RenderCommand::DrawIndexed(RendererAPI::Topology::Triangles, s_Data.quadCount * 6u);

		++s_Data.stats.DrawCalls;
		s_Data.quadInsert = s_Data.quadVertexData;
		s_Data.quadCount = 0u;
		s_Data.quadTextureIndex = 1u;
	}

	void Renderer2D::SubmitCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, uint32_t entityID)
	{
		if (s_Data.circleCount >= c_MaxQuads)
			DrawCircles();

		static constexpr glm::vec4 position[4] = {
			{ -0.5f,  0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.0f, 1.0f }
		};
		for (int i = 0; i < 4; ++i)
		{
			s_Data.circleInsert->pos = transform * position[i];
			s_Data.circleInsert->color = color;
			s_Data.circleInsert->thickness = thickness;
			s_Data.circleInsert->fade = fade;
			s_Data.circleInsert->id = entityID;
			++s_Data.circleInsert;
		}
		++s_Data.circleCount;
		++s_Data.stats.CircleCount;
	}
	void Renderer2D::SubmitCircle(const glm::vec2& pos, float radius, const glm::vec4& color, float thickness, float fade, uint32_t entityID)
	{
		SubmitCircle(glm::vec3(pos, 0.0f), radius, color, thickness, fade, entityID);
	}
	void Renderer2D::SubmitCircle(const glm::vec3& pos, float radius, const glm::vec4& color, float thickness, float fade, uint32_t entityID)
	{
		if (s_Data.circleCount >= c_MaxQuads)
			DrawCircles();

		static constexpr glm::vec3 position[4] = {
			{ -0.5f,  0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f }
		};
		for (int i = 0; i < 4; ++i)
		{
			s_Data.circleInsert->pos = glm::vec4(position[i] * radius + pos, 1.0f);
			s_Data.circleInsert->color = color;
			s_Data.circleInsert->thickness = thickness;
			s_Data.circleInsert->fade = fade;
			s_Data.circleInsert->id = entityID;
			++s_Data.circleInsert;
		}
		++s_Data.circleCount;
		++s_Data.stats.CircleCount;
	}
	void Renderer2D::DrawCircles()
	{
		if (s_Data.circleCount == 0u)
			return;

		s_Data.circleInputLayout->GetVertexBuffer()->SetData(s_Data.circleVertexData, c_CircleBufferSize);
		s_Data.circleShader->Bind();
		s_Data.viewProj->Bind();
		s_Data.circleInputLayout->Bind();
		RenderCommand::DrawIndexed(RendererAPI::Topology::Triangles, s_Data.circleCount * 6u);
		++s_Data.stats.DrawCalls;
		s_Data.circleInsert = s_Data.circleVertexData;
		s_Data.circleCount = 0u;
	}

	Renderer2D::Stats Renderer2D::GetStats() noexcept
	{
		return s_Data.stats;
	}
	void Renderer2D::ResetStats() noexcept
	{
		s_Data.stats.QuadCount = 0u;
		s_Data.stats.CircleCount = 0u;
		s_Data.stats.DrawCalls = 0u;
	}
}
