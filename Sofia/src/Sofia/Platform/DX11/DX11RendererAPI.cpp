#include <pch.h>
#include "DX11RendererAPI.h"

#include "Sofia/Platform/DX11/DX11Context.h"
#include "Sofia/Renderer/Renderer.h"

namespace Sofia {

	static D3D11_COMPARISON_FUNC GetCompFunc(RendererAPI::ComparisonFunc compFunc)
	{
		switch (compFunc)
		{
		case Sofia::RendererAPI::ComparisonFunc::Never:				return D3D11_COMPARISON_NEVER;
		case Sofia::RendererAPI::ComparisonFunc::Less:				return D3D11_COMPARISON_LESS;
		case Sofia::RendererAPI::ComparisonFunc::Equal:				return D3D11_COMPARISON_EQUAL;
		case Sofia::RendererAPI::ComparisonFunc::LessOrEqual:		return D3D11_COMPARISON_LESS_EQUAL;
		case Sofia::RendererAPI::ComparisonFunc::Greater:			return D3D11_COMPARISON_GREATER;
		case Sofia::RendererAPI::ComparisonFunc::GreaterOrEqual:	return D3D11_COMPARISON_GREATER_EQUAL;
		case Sofia::RendererAPI::ComparisonFunc::NotEqual:			return D3D11_COMPARISON_NOT_EQUAL;
		case Sofia::RendererAPI::ComparisonFunc::Always:			return D3D11_COMPARISON_ALWAYS;
		}
		SOF_CORE_THROW_INFO("Invalid comparison function");
		return D3D11_COMPARISON_ALWAYS;
	}
	static D3D11_STENCIL_OP GetStencilOp(RendererAPI::StencilOperation op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::StencilOperation::Keep:			return D3D11_STENCIL_OP_KEEP;
		case Sofia::RendererAPI::StencilOperation::Zero:			return D3D11_STENCIL_OP_ZERO;
		case Sofia::RendererAPI::StencilOperation::Replace:			return D3D11_STENCIL_OP_REPLACE;
		case Sofia::RendererAPI::StencilOperation::Increment:		return D3D11_STENCIL_OP_INCR;
		case Sofia::RendererAPI::StencilOperation::IncrementClamp:	return D3D11_STENCIL_OP_INCR_SAT;
		case Sofia::RendererAPI::StencilOperation::Decrement:		return D3D11_STENCIL_OP_DECR;
		case Sofia::RendererAPI::StencilOperation::DecrementClamp:	return D3D11_STENCIL_OP_DECR_SAT;
		case Sofia::RendererAPI::StencilOperation::Invert:			return D3D11_STENCIL_OP_INVERT;
		}
		SOF_CORE_THROW_INFO("Invalid stencil operation");
		return D3D11_STENCIL_OP_KEEP;
	}
	static D3D11_BLEND GetBlendOption(RendererAPI::BlendOption op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::BlendOption::Zero:						return D3D11_BLEND_ONE;
		case Sofia::RendererAPI::BlendOption::One:						return D3D11_BLEND_ONE;
		case Sofia::RendererAPI::BlendOption::SourceColor:				return D3D11_BLEND_SRC_COLOR;
		case Sofia::RendererAPI::BlendOption::SourceColorInvert:		return D3D11_BLEND_INV_SRC_COLOR;
		case Sofia::RendererAPI::BlendOption::SourceAlpha:				return D3D11_BLEND_SRC_ALPHA;
		case Sofia::RendererAPI::BlendOption::SourceAlphaInvert:		return D3D11_BLEND_INV_SRC_ALPHA;
		case Sofia::RendererAPI::BlendOption::DestinationColor:			return D3D11_BLEND_DEST_COLOR;
		case Sofia::RendererAPI::BlendOption::DestinationColorInvert:	return D3D11_BLEND_INV_DEST_COLOR;
		case Sofia::RendererAPI::BlendOption::DestinationAlpha:			return D3D11_BLEND_DEST_ALPHA;
		case Sofia::RendererAPI::BlendOption::DestinationAlphaInvert:	return D3D11_BLEND_INV_DEST_ALPHA;
		case Sofia::RendererAPI::BlendOption::BlendFactor:				return D3D11_BLEND_BLEND_FACTOR;
		case Sofia::RendererAPI::BlendOption::BlendFactorInvert:		return D3D11_BLEND_INV_BLEND_FACTOR;
		}
		SOF_CORE_THROW_INFO("Invalid blend option");
		return D3D11_BLEND_ONE;
	}
	static D3D11_BLEND_OP GetBlendOperation(RendererAPI::BlendOperation op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::BlendOperation::Add:				return D3D11_BLEND_OP_ADD;
		case Sofia::RendererAPI::BlendOperation::Subtract:			return D3D11_BLEND_OP_SUBTRACT;
		case Sofia::RendererAPI::BlendOperation::ReverseSubtract:	return D3D11_BLEND_OP_REV_SUBTRACT;
		case Sofia::RendererAPI::BlendOperation::Min:				return D3D11_BLEND_OP_MIN;
		case Sofia::RendererAPI::BlendOperation::Max:				return D3D11_BLEND_OP_MAX;
		}
		SOF_CORE_THROW_INFO("Invalid blend operation");
		return D3D11_BLEND_OP_ADD;
	}

	void DX11RendererAPI::Init()
	{
		InitShaders();

		m_DepthStencilDesc.DepthEnable = true;
		m_DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		m_DepthStencilDesc.StencilEnable = false;
		m_DepthStencilDesc.StencilReadMask = 0xffu;
		m_DepthStencilDesc.StencilWriteMask = 0xffu;
		m_DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_StencilRefVal = 0xffu;
		m_DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		RecreateDepthStencilState();

		m_BlendDesc.AlphaToCoverageEnable = false;
		m_BlendDesc.IndependentBlendEnable = true;
	}
	void DX11RendererAPI::Shutdown()
	{
		m_DepthStencilState.Reset();
		m_RasterizerState.Reset();
		m_BlendState.Reset();
	}

	void DX11RendererAPI::Draw(Topology topology, uint32_t verticesCount)
	{
		auto context = DX11Context::GetContextFromApplication()->GetContext();
		SetTopology(context, topology);
		context->Draw(verticesCount, 0u);
	}
	void DX11RendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount)
	{
		auto context = DX11Context::GetContextFromApplication()->GetContext();
		SetTopology(context, topology);
		context->DrawIndexed(indicesCount, 0u, 0u);

	}
	void DX11RendererAPI::InitShaders()
	{
		std::string vertexQuad = R"(
			struct VSOut
			{
				float4 pos : SV_Position;
				float4 color : Color;
				float2 uv : UV;
				nointerpolation int tid : TID;
				float tillingFactor : TillingFactor;
				nointerpolation uint id : ID;
			};
			cbuffer ConstBuf
			{
				uniform matrix<float, 4, 4> u_ViewProjMat;
			}
			VSOut main(float4 pos : Position, float4 color : Color, float2 uv : UV, int tid : TID, float tillingFactor : TillingFactor, uint id : EntityID)
			{
				VSOut output;
				output.pos = mul(pos, u_ViewProjMat);
				output.color = color;
				output.uv = uv;
				output.tid = tid;
				output.tillingFactor = tillingFactor;
				output.id = id;
				return output;
			}
		)";

		std::string fragmentQuad = R"(
			struct FSIn
			{
				float4 pos : SV_Position;
				float4 color : Color;
				float2 uv : UV;
				nointerpolation int tid : TID;
				float tillingFactor : TillingFactor;
				nointerpolation uint id : ID;
			};
			struct FSOut
			{
				float4 color : SV_Target0;
				uint id : SV_Target1;
			};
			Texture2D<float4> u_Textures[16];
			SamplerState u_Samplers[16];
			float4 GetDataFromTexture(int tid, float2 uv, float tillingFactor)
			{
				float4 output = u_Textures[0].Sample(u_Samplers[0], uv * tillingFactor) * (1 - abs(sign(tid - 0)));
				output += u_Textures[1].Sample(u_Samplers[1], uv * tillingFactor) * (1 - abs(sign(tid - 1)));
				output += u_Textures[2].Sample(u_Samplers[2], uv * tillingFactor) * (1 - abs(sign(tid - 2)));
				output += u_Textures[3].Sample(u_Samplers[3], uv * tillingFactor) * (1 - abs(sign(tid - 3)));
				output += u_Textures[4].Sample(u_Samplers[4], uv * tillingFactor) * (1 - abs(sign(tid - 4)));
				output += u_Textures[5].Sample(u_Samplers[5], uv * tillingFactor) * (1 - abs(sign(tid - 5)));
				output += u_Textures[6].Sample(u_Samplers[6], uv * tillingFactor) * (1 - abs(sign(tid - 6)));
				output += u_Textures[7].Sample(u_Samplers[7], uv * tillingFactor) * (1 - abs(sign(tid - 7)));
				output += u_Textures[8].Sample(u_Samplers[8], uv * tillingFactor) * (1 - abs(sign(tid - 8)));
				output += u_Textures[9].Sample(u_Samplers[9], uv * tillingFactor) * (1 - abs(sign(tid - 9)));
				output += u_Textures[10].Sample(u_Samplers[10], uv * tillingFactor) * (1 - abs(sign(tid - 10)));
				output += u_Textures[11].Sample(u_Samplers[11], uv * tillingFactor) * (1 - abs(sign(tid - 11)));
				output += u_Textures[12].Sample(u_Samplers[12], uv * tillingFactor) * (1 - abs(sign(tid - 12)));
				output += u_Textures[13].Sample(u_Samplers[13], uv * tillingFactor) * (1 - abs(sign(tid - 13)));
				output += u_Textures[14].Sample(u_Samplers[14], uv * tillingFactor) * (1 - abs(sign(tid - 14)));
				output += u_Textures[15].Sample(u_Samplers[15], uv * tillingFactor) * (1 - abs(sign(tid - 15)));
				return output;
			}
			FSOut main(FSIn input)
			{
				FSOut output;
				output.color = input.color * GetDataFromTexture(input.tid, input.uv, input.tillingFactor);
				output.id = input.id;
				return output;
			}
		)";
		Renderer::GetShaderLibrary().Load("quadShader", vertexQuad, fragmentQuad);

		std::string vertexCircle = R"(
			struct VSOut
			{
				float4 pos : SV_Position;
				float4 localPos : LocalPos;
				float4 color : Color;
				nointerpolation float thickness : Thickness;
				nointerpolation float fade : Fade;
				nointerpolation uint id : ID;
			};
			cbuffer ConstBuf
			{
				uniform matrix<float, 4, 4> u_ViewProjMat;
			}
			static const float4 localPos[4] = {
				{ -1.0f,  1.0f, 0.0f, 0.0f },
				{  1.0f,  1.0f, 0.0f, 0.0f },
				{  1.0f, -1.0f, 0.0f, 0.0f },
				{ -1.0f, -1.0f, 0.0f, 0.0f }
			};
			VSOut main(float4 pos : Position, float4 color : Color, float thickness : Thickness, float fade : Fade, uint id : EntityID, uint vid : SV_VertexID)
			{
				VSOut output;
				output.pos = mul(pos, u_ViewProjMat);
				output.localPos = localPos[vid % 4];
				output.color = color;
				output.thickness = thickness;
				output.fade = fade;
				output.id = id;
				return output;
			}
		)";
		std::string fragmentCircle = R"(
			struct FSIn
			{
				float4 pos : SV_Position;
				float4 localPos : LocalPos;
				float4 color : Color;
				nointerpolation float thickness : Thickness;
				nointerpolation float fade : Fade;
				nointerpolation uint id : ID;
			};
			struct FSOut
			{
				float4 color : SV_Target0;
				uint id : SV_Target1;
			};
			FSOut main(FSIn input)
			{
				const float distance = 1.0f - length(input.localPos);
				float alpha = smoothstep(0.0f, input.fade, distance) * smoothstep(input.thickness + input.fade, input.thickness, distance);
				if (alpha == 0.0f)
					discard;
				FSOut output;
				output.color = input.color;
				output.color.a *= alpha;
				output.id = input.id;
				return output;
			}
		)";
		Renderer::GetShaderLibrary().Load("circleShader", vertexCircle, fragmentCircle);
	}
	void DX11RendererAPI::SetTopology(ComPtr<ID3D11DeviceContext> context, Topology topology)
	{
		switch (topology)
		{
		case Sofia::RendererAPI::Topology::Points:
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			return;
		case Sofia::RendererAPI::Topology::Lines:
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			return;
		case Sofia::RendererAPI::Topology::LineStrip:
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			return;
		case Sofia::RendererAPI::Topology::Triangles:
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			return;
		case Sofia::RendererAPI::Topology::TriangleStrip:
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			return;
		}
		SOF_CORE_THROW_INFO("Unknown topology");
	}

	void DX11RendererAPI::SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc)
	{
		m_DepthStencilDesc.DepthEnable = enable;
		m_DepthStencilDesc.DepthWriteMask = writeMask ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		m_DepthStencilDesc.DepthFunc = GetCompFunc(compFunc);
		RecreateDepthStencilState();
	}
	void DX11RendererAPI::SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc, ComparisonFunc backFaceFunc, uint32_t stencilRefVal)
	{
		m_DepthStencilDesc.StencilEnable = enable;
		m_DepthStencilDesc.StencilReadMask = readMask;
		m_DepthStencilDesc.StencilWriteMask = writeMask;
		m_DepthStencilDesc.FrontFace.StencilFunc = GetCompFunc(frontFaceFunc);
		m_DepthStencilDesc.BackFace.StencilFunc = GetCompFunc(backFaceFunc);
		m_StencilRefVal = stencilRefVal;
		RecreateDepthStencilState();
	}
	void DX11RendererAPI::SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass)
	{
		m_DepthStencilDesc.FrontFace.StencilFailOp = GetStencilOp(stencilFail);
		m_DepthStencilDesc.FrontFace.StencilDepthFailOp = GetStencilOp(depthFail);
		m_DepthStencilDesc.FrontFace.StencilPassOp = GetStencilOp(pass);
		RecreateDepthStencilState();
	}
	void DX11RendererAPI::SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass)
	{
		m_DepthStencilDesc.BackFace.StencilFailOp = GetStencilOp(stencilFail);
		m_DepthStencilDesc.BackFace.StencilDepthFailOp = GetStencilOp(depthFail);
		m_DepthStencilDesc.BackFace.StencilPassOp = GetStencilOp(pass);
		RecreateDepthStencilState();
	}
	void DX11RendererAPI::RecreateDepthStencilState()
	{
		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateDepthStencilState(&m_DepthStencilDesc, &m_DepthStencilState));
		DX11Context::GetContextFromApplication()->GetContext()->OMSetDepthStencilState(m_DepthStencilState.Get(), m_StencilRefVal);
	}
	void DX11RendererAPI::SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise)
	{
		SOF_CORE_ASSERT(fillMode == TriangleFillMode::Full || fillMode == TriangleFillMode::Wireframe, "Invalid triangle fill mode");
		SOF_CORE_ASSERT(cullMode >= TriangleCullMode::DrawAll && cullMode <= TriangleCullMode::DrawBackFace, "Invalid triangle cull mode");
		D3D11_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc.FillMode = fillMode == TriangleFillMode::Wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = cullMode == TriangleCullMode::DrawFrontFace ? D3D11_CULL_BACK : cullMode == TriangleCullMode::DrawAll ? D3D11_CULL_NONE : D3D11_CULL_FRONT;
		rasterizerDesc.FrontCounterClockwise = isFrontFaceCounterClockwise;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.AntialiasedLineEnable = false;

		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
		DX11Context::GetContextFromApplication()->GetContext()->RSSetState(m_RasterizerState.Get());
	}
	void DX11RendererAPI::SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor)
	{
		SOF_CORE_ASSERT(0 <= i && i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Exceeding available number of simultaneous render targets");
		m_BlendDesc.RenderTarget[i].BlendEnable = enable;
		m_BlendDesc.RenderTarget[i].SrcBlend = GetBlendOption(sourceBlend);
		m_BlendDesc.RenderTarget[i].DestBlend = GetBlendOption(destinationBlend);
		m_BlendDesc.RenderTarget[i].BlendOp = GetBlendOperation(operation);
		m_BlendDesc.RenderTarget[i].SrcBlendAlpha = GetBlendOption(sourceAlphaBlend);
		m_BlendDesc.RenderTarget[i].DestBlendAlpha = GetBlendOption(destinationAlphaBlend);
		m_BlendDesc.RenderTarget[i].BlendOpAlpha = GetBlendOperation(alphaOperation);
		m_BlendDesc.RenderTarget[i].RenderTargetWriteMask = writeMask;

		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBlendState(&m_BlendDesc, &m_BlendState));
		DX11Context::GetContextFromApplication()->GetContext()->OMSetBlendState(m_BlendState.Get(), glm::value_ptr(blendFactor), 0xffffffffu);
	}
}
