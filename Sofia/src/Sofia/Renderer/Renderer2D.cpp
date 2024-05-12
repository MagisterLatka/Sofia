#include <pch.h>
#include "Renderer2D.h"

#include "Sofia/Renderer/InputLayout.h"
#include "Sofia/Renderer/Renderer.h"
#include "Sofia/Renderer/RenderCommand.h"

namespace Sofia {

	struct VertexData
	{
		glm::vec4 pos;
		glm::vec4 color;
		glm::vec2 uv;
		int tid;
		float tillingFactor;
		uint32_t id;
		glm::ivec3 padding;
	};

	static constexpr uint32_t c_MaxQuads = 10000u;
	static constexpr uint32_t c_QuadBufferSize = c_MaxQuads * 4 * sizeof(VertexData);
	static constexpr uint32_t c_MaxTextures = 16u;

	struct Renderer2DData
	{
		VertexData* quadVertexData = nullptr;
		VertexData* quadInsert = nullptr;
		uint32_t quadCount = 0u;
		Ref<InputLayout> quadInputLayout;
		std::array<Ref<Texture2D>, c_MaxTextures> quadTextures;
		uint32_t quadTextureIndex = 1u;

		Ref<Shader> shader;
		Ref<ConstantBuffer> viewProj;

		Renderer2D::Stats stats;
	};
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.quadVertexData = new VertexData[c_MaxQuads * 4];
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

		s_Data.shader = Renderer::GetShaderLibrary().Get("2D");
		UniformBuffer<sizeof(glm::mat4), 1> uniformBuffer;
		uniformBuffer.Push("u_ViewProjMat", glm::mat4(1.0f));
		s_Data.viewProj = ConstantBuffer::Create(BufferShaderBinding::Vertex, uniformBuffer);

		BufferLayout layout = {
			{ "Position", BufferLayoutElementDataType::Float4 },
			{ "Color", BufferLayoutElementDataType::Float4 },
			{ "UV", BufferLayoutElementDataType::Float2 },
			{ "TID", BufferLayoutElementDataType::Int },
			{ "TillingFactor", BufferLayoutElementDataType::Float },
			{ "EntityID", BufferLayoutElementDataType::UInt4 }
		};
		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(layout, s_Data.quadVertexData, c_QuadBufferSize);
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indices, c_MaxQuads * 6 * sizeof(uint32_t));
		s_Data.quadInputLayout = InputLayout::Create({ quadVBO }, s_Data.shader, quadIBO);
		delete[] indices;

		uint32_t texData = 0xffffffffu;
		s_Data.quadTextures[0] = Texture2D::Create(1u, 1u, &texData);
	}
	void Renderer2D::Shutdown()
	{
		for (uint32_t i = 0; i < c_MaxTextures; ++i)
			s_Data.quadTextures[i].Reset();
		s_Data.quadInputLayout.Reset();
		s_Data.viewProj.Reset();
		s_Data.shader.Reset();
		delete[] s_Data.quadVertexData;
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
		s_Data.shader->Bind();
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

	Renderer2D::Stats Renderer2D::GetStats() noexcept
	{
		return s_Data.stats;
	}
	void Renderer2D::ResetStats() noexcept
	{
		s_Data.stats.QuadCount = 0u;
		s_Data.stats.DrawCalls = 0u;
	}
}
