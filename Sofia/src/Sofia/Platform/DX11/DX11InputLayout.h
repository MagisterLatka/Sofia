#pragma once

#include "Sofia/Renderer/InputLayout.h"
#include "Sofia/Platform/DX11/DX11GraphicsBuffers.h"
#include "Sofia/Platform/DX11/DX11Shader.h"
#include <d3d11.h>

namespace Sofia {

	class DX11InputLayout : public InputLayout
	{
	public:
		DX11InputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer);
		~DX11InputLayout();

		virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index) const noexcept override { return m_VertexBuffers[index]; }
		virtual Ref<IndexBuffer> GetIndexBuffer() const noexcept override { return m_IndexBuffer; }

		virtual void Bind() const noexcept override;
	private:
		void Create();
	private:
		ComPtr<ID3D11InputLayout> m_InputLayout;
		std::vector<Ref<DX11VertexBuffer>> m_VertexBuffers;
		uint32_t* m_Strides;
		ID3D11Buffer** m_Buffers;
		Ref<DX11IndexBuffer> m_IndexBuffer;
		Ref<DX11Shader> m_Shader;
	};
}
