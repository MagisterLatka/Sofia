#include <pch.h>
#include "OpenGLRendererAPI.h"

#include <Sofia/Renderer/Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sofia {

	static GLenum GetComparisonFunc(RendererAPI::ComparisonFunc func)
	{
		switch (func)
		{
		case Sofia::RendererAPI::ComparisonFunc::Never:				return GL_NEVER;
		case Sofia::RendererAPI::ComparisonFunc::Less:				return GL_LESS;
		case Sofia::RendererAPI::ComparisonFunc::Equal:				return GL_EQUAL;
		case Sofia::RendererAPI::ComparisonFunc::LessOrEqual:		return GL_LEQUAL;
		case Sofia::RendererAPI::ComparisonFunc::Greater:			return GL_GREATER;
		case Sofia::RendererAPI::ComparisonFunc::GreaterOrEqual:	return GL_GEQUAL;
		case Sofia::RendererAPI::ComparisonFunc::NotEqual:			return GL_NOTEQUAL;
		case Sofia::RendererAPI::ComparisonFunc::Always:			return GL_ALWAYS;
		}
		SOF_CORE_THROW_INFO("Invalid comparison function");
		return GL_LESS;
	}
	static GLenum GetStencilOp(RendererAPI::StencilOperation op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::StencilOperation::Keep:			return GL_KEEP;
		case Sofia::RendererAPI::StencilOperation::Zero:			return GL_ZERO;
		case Sofia::RendererAPI::StencilOperation::Replace:			return GL_REPLACE;
		case Sofia::RendererAPI::StencilOperation::Increment:		return GL_INCR;
		case Sofia::RendererAPI::StencilOperation::IncrementClamp:	return GL_INCR_WRAP;
		case Sofia::RendererAPI::StencilOperation::Decrement:		return GL_DECR;
		case Sofia::RendererAPI::StencilOperation::DecrementClamp:	return GL_DECR_WRAP;
		case Sofia::RendererAPI::StencilOperation::Invert:			return GL_INVERT;
		}
		SOF_CORE_THROW_INFO("Invalid stencil operation");
		return GL_KEEP;
	}
	static GLenum GetBlendOption(RendererAPI::BlendOption op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::BlendOption::Zero:						return GL_ZERO;
		case Sofia::RendererAPI::BlendOption::One:						return GL_ONE;
		case Sofia::RendererAPI::BlendOption::SourceColor:				return GL_SRC_COLOR;
		case Sofia::RendererAPI::BlendOption::SourceColorInvert:		return GL_ONE_MINUS_SRC_COLOR;
		case Sofia::RendererAPI::BlendOption::SourceAlpha:				return GL_SRC_ALPHA;
		case Sofia::RendererAPI::BlendOption::SourceAlphaInvert:		return GL_ONE_MINUS_SRC_ALPHA;
		case Sofia::RendererAPI::BlendOption::DestinationColor:			return GL_DST_COLOR;
		case Sofia::RendererAPI::BlendOption::DestinationColorInvert:	return GL_ONE_MINUS_DST_COLOR;
		case Sofia::RendererAPI::BlendOption::DestinationAlpha:			return GL_DST_ALPHA;
		case Sofia::RendererAPI::BlendOption::DestinationAlphaInvert:	return GL_ONE_MINUS_DST_ALPHA;
		case Sofia::RendererAPI::BlendOption::BlendFactor:				return GL_CONSTANT_COLOR;
		case Sofia::RendererAPI::BlendOption::BlendFactorInvert:		return GL_ONE_MINUS_CONSTANT_COLOR;
		}
		SOF_CORE_THROW_INFO("Invalid blend option");
		return GL_ONE;
	}
	static GLenum GetBlendOp(RendererAPI::BlendOperation op)
	{
		switch (op)
		{
		case Sofia::RendererAPI::BlendOperation::Add:				return GL_FUNC_ADD;
		case Sofia::RendererAPI::BlendOperation::Subtract:			return GL_FUNC_SUBTRACT;
		case Sofia::RendererAPI::BlendOperation::ReverseSubtract:	return GL_FUNC_REVERSE_SUBTRACT;
		case Sofia::RendererAPI::BlendOperation::Min:				return GL_MIN;
		case Sofia::RendererAPI::BlendOperation::Max:				return GL_MAX;
		}
		SOF_CORE_THROW_INFO("Invalid blend operation");
		return GL_FUNC_ADD;
	}
	static void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* Source;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				Source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		Source = "WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	Source = "SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		Source = "THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		Source = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:
		default:								Source = "UNKNOWN"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			SOF_CORE_CRITICAL("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			SOF_CORE_ERROR("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_LOW:				SOF_CORE_WARN("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	SOF_CORE_TRACE("({0}): {1}", Source, message); break;
		}
	}
	void OpenGLRendererAPI::Init()
	{
#ifdef SOF_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

		InitShaders();
	}
	void OpenGLRendererAPI::Shutdown()
	{
		
	}

	void OpenGLRendererAPI::Draw(Topology topology, uint32_t verticesCount)
	{
		glDrawArrays(GetTopology(topology), 0, verticesCount);
	}
	void OpenGLRendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount)
	{
		glDrawElements(GetTopology(topology), indicesCount, GL_UNSIGNED_INT, nullptr);
	}
	int OpenGLRendererAPI::GetTopology(Topology topology)
	{
		switch (topology)
		{
		case Sofia::RendererAPI::Topology::None: SOF_CORE_ASSERT("None topology cannot be used"); return -1;
		case Sofia::RendererAPI::Topology::Points: return GL_POINTS;
		case Sofia::RendererAPI::Topology::Lines: return GL_LINES;
		case Sofia::RendererAPI::Topology::LineStrip: return GL_LINE_STRIP;
		case Sofia::RendererAPI::Topology::Triangles: return GL_TRIANGLES;
		case Sofia::RendererAPI::Topology::TriangleStrip: return GL_TRIANGLE_STRIP;
		}
		SOF_CORE_ASSERT("Unknown topology");
		return -1;
	}
	void OpenGLRendererAPI::InitShaders()
	{
		std::string vertexQuad = R"(
			#version 460 core

			layout(location = 0) in vec4 i_Pos;
			layout(location = 1) in vec4 i_Color;
			layout(location = 2) in vec2 i_UV;
			layout(location = 3) in int i_TID;
			layout(location = 4) in float i_TillingFactor;
			layout(location = 5) in uvec4 i_ID;

			layout(std140, binding = 0) uniform RendererData
			{
				mat4 u_ViewProjMat;
			};

			out Data {
				vec4 color;
				vec2 uv;
				flat int tid;
				flat float tillingFactor;
				flat uint id;
			} vs_out;

			void main()
			{
				gl_Position = u_ViewProjMat * i_Pos;
				vs_out.color = i_Color;
				vs_out.uv = i_UV;
				vs_out.tid = i_TID;
				vs_out.tillingFactor = i_TillingFactor;
				vs_out.id = i_ID.x;
			}
		)";

		std::string fragmentQuad = R"(
			#version 460 core

			layout(location = 0) out vec4 o_Color;
			layout(location = 1) out uint o_ID;

			in Data {
				vec4 color;
				vec2 uv;
				flat int tid;
				flat float tillingFactor;
				flat uint id;
			} fs_in;

			layout(binding = 0) uniform sampler2D u_Tex0;
			layout(binding = 1) uniform sampler2D u_Tex1;
			layout(binding = 2) uniform sampler2D u_Tex2;
			layout(binding = 3) uniform sampler2D u_Tex3;
			layout(binding = 4) uniform sampler2D u_Tex4;
			layout(binding = 5) uniform sampler2D u_Tex5;
			layout(binding = 6) uniform sampler2D u_Tex6;
			layout(binding = 7) uniform sampler2D u_Tex7;
			layout(binding = 8) uniform sampler2D u_Tex8;
			layout(binding = 9) uniform sampler2D u_Tex9;
			layout(binding = 10) uniform sampler2D u_Tex10;
			layout(binding = 11) uniform sampler2D u_Tex11;
			layout(binding = 12) uniform sampler2D u_Tex12;
			layout(binding = 13) uniform sampler2D u_Tex13;
			layout(binding = 14) uniform sampler2D u_Tex14;
			layout(binding = 15) uniform sampler2D u_Tex15;

			vec4 GetDataFromTexture(int tid, vec2 uv, float tillingFactor)
			{
				vec4 color = texture(u_Tex0, uv * tillingFactor) * (1 - abs(sign(tid - 0)));
				color += texture(u_Tex1, uv * tillingFactor) * (1 - abs(sign(tid - 1)));
				color += texture(u_Tex2, uv * tillingFactor) * (1 - abs(sign(tid - 2)));
				color += texture(u_Tex3, uv * tillingFactor) * (1 - abs(sign(tid - 3)));
				color += texture(u_Tex4, uv * tillingFactor) * (1 - abs(sign(tid - 4)));
				color += texture(u_Tex5, uv * tillingFactor) * (1 - abs(sign(tid - 5)));
				color += texture(u_Tex6, uv * tillingFactor) * (1 - abs(sign(tid - 6)));
				color += texture(u_Tex7, uv * tillingFactor) * (1 - abs(sign(tid - 7)));
				color += texture(u_Tex8, uv * tillingFactor) * (1 - abs(sign(tid - 8)));
				color += texture(u_Tex9, uv * tillingFactor) * (1 - abs(sign(tid - 9)));
				color += texture(u_Tex10, uv * tillingFactor) * (1 - abs(sign(tid - 10)));
				color += texture(u_Tex11, uv * tillingFactor) * (1 - abs(sign(tid - 11)));
				color += texture(u_Tex12, uv * tillingFactor) * (1 - abs(sign(tid - 12)));
				color += texture(u_Tex13, uv * tillingFactor) * (1 - abs(sign(tid - 13)));
				color += texture(u_Tex14, uv * tillingFactor) * (1 - abs(sign(tid - 14)));
				color += texture(u_Tex15, uv * tillingFactor) * (1 - abs(sign(tid - 15)));
				return color;
			}

			void main()
			{
				o_Color = fs_in.color * GetDataFromTexture(fs_in.tid, fs_in.uv, fs_in.tillingFactor);
				o_ID = fs_in.id;
			}
		)";
		Renderer::GetShaderLibrary().Load("quadShader", vertexQuad, fragmentQuad);

		std::string vertexCircle= R"(
			#version 460 core

			layout(location = 0) in vec4 i_Pos;
			layout(location = 1) in vec4 i_Color;
			layout(location = 2) in float i_Thickness;
			layout(location = 3) in float i_Fade;
			layout(location = 4) in uvec2 i_ID;

			layout(std140, binding = 0) uniform RendererData
			{
				mat4 u_ViewProjMat;
			};

			out Data {
				vec4 localPos;
				vec4 color;
				flat float thickness;
				flat float fade;
				flat uint id;
			} vs_out;

			const vec4 localPos[4] = {
				{ -1.0f,  1.0f, 0.0f, 0.0f },
				{  1.0f,  1.0f, 0.0f, 0.0f },
				{  1.0f, -1.0f, 0.0f, 0.0f },
				{ -1.0f, -1.0f, 0.0f, 0.0f }
			};

			void main()
			{
				gl_Position = u_ViewProjMat * i_Pos;
				vs_out.localPos = localPos[gl_VertexID % 4];
				vs_out.color = i_Color;
				vs_out.thickness = i_Thickness;
				vs_out.fade = i_Fade;
				vs_out.id = i_ID.x;
			}
		)";

		std::string fragmentCircle = R"(
			#version 460 core

			layout(location = 0) out vec4 o_Color;
			layout(location = 1) out uint o_ID;

			in Data {
				vec4 localPos;
				vec4 color;
				flat float thickness;
				flat float fade;
				flat uint id;
			} fs_in;

			void main()
			{
				const float distance = 1.0f - length(fs_in.localPos);
				float alpha = smoothstep(0.0f, fs_in.fade, distance) * smoothstep(fs_in.thickness + fs_in.fade, fs_in.thickness, distance);
				if (alpha == 0.0f)
					discard;
				o_Color = fs_in.color * vec4(1.0f, 1.0f, 1.0f, alpha);
				o_ID = fs_in.id;
			}
		)";
		Renderer::GetShaderLibrary().Load("circleShader", vertexCircle, fragmentCircle);
	}

	void OpenGLRendererAPI::SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc)
	{
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		glDepthMask(writeMask);
		glDepthFunc(GetComparisonFunc(compFunc));
	}
	void OpenGLRendererAPI::SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc, ComparisonFunc backFaceFunc, uint32_t stencilRefVal)
	{
		if (enable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		glStencilMask(writeMask);
		glStencilFuncSeparate(GL_FRONT, GetComparisonFunc(frontFaceFunc), stencilRefVal, readMask);
		glStencilFuncSeparate(GL_BACK, GetComparisonFunc(backFaceFunc), stencilRefVal, readMask);
	}
	void OpenGLRendererAPI::SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass)
	{
		glStencilOpSeparate(GL_FRONT, GetStencilOp(stencilFail), GetStencilOp(depthFail), GetStencilOp(pass));
	}
	void OpenGLRendererAPI::SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass)
	{
		glStencilOpSeparate(GL_BACK, GetStencilOp(stencilFail), GetStencilOp(depthFail), GetStencilOp(pass));
	}
	void OpenGLRendererAPI::SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise)
	{
		GLenum fill = 0;
		switch (fillMode)
		{
		case Sofia::RendererAPI::TriangleFillMode::Full:
			fill = GL_FILL;
			break;
		case Sofia::RendererAPI::TriangleFillMode::Wireframe:
			fill = GL_LINE;
			break;
		default:
			SOF_CORE_THROW_INFO("Invalid triangle fill mode");
			break;
		}
		glPolygonMode(GL_FRONT_AND_BACK, fill);

		switch (cullMode)
		{
		case Sofia::RendererAPI::TriangleCullMode::DrawAll:
			glDisable(GL_CULL_FACE);
			break;
		case Sofia::RendererAPI::TriangleCullMode::DrawFrontFace:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case Sofia::RendererAPI::TriangleCullMode::DrawBackFace:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		default:
			SOF_CORE_THROW_INFO("Invalid triangle cull mode");
			break;
		}

		if (isFrontFaceCounterClockwise)
			glFrontFace(GL_CCW);
		else
			glFrontFace(GL_CW);
	}
	void OpenGLRendererAPI::SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation, BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor)
	{
		if (enable)
			glEnablei(GL_BLEND, i);
		else
			glDisablei(GL_BLEND, i);

		glBlendFuncSeparatei(i, GetBlendOption(sourceBlend), GetBlendOption(destinationBlend), GetBlendOption(sourceAlphaBlend), GetBlendOption(destinationAlphaBlend));
		glBlendEquationSeparatei(i, GetBlendOp(operation), GetBlendOp(alphaOperation));
		glBlendColor(blendFactor.r, blendFactor.g, blendFactor.b, blendFactor.a);
	}
}
