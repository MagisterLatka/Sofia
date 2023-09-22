#include <pch.h>
#include "DX11RendererAPI.h"

#include "Sofia/Platform/DX11/DX11Context.h"

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
	}
	void DX11RendererAPI::SetBlendOptions(bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor)
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = enable;
		blendDesc.RenderTarget[0].SrcBlend = GetBlendOption(sourceBlend);
		blendDesc.RenderTarget[0].DestBlend = GetBlendOption(destinationBlend);
		blendDesc.RenderTarget[0].BlendOp = GetBlendOperation(operation);
		blendDesc.RenderTarget[0].SrcBlendAlpha = GetBlendOption(sourceAlphaBlend);
		blendDesc.RenderTarget[0].DestBlendAlpha = GetBlendOption(destinationAlphaBlend);
		blendDesc.RenderTarget[0].BlendOpAlpha = GetBlendOperation(alphaOperation);
		blendDesc.RenderTarget[0].RenderTargetWriteMask = writeMask;

		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBlendState(&blendDesc, &m_BlendState));
		DX11Context::GetContextFromApplication()->GetContext()->OMSetBlendState(m_BlendState.Get(), glm::value_ptr(blendFactor), 0xffffffffu);
	}
}
