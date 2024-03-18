#include <pch.h>
#include "OpenGLRendererAPI.h"

#include <Sofia/Renderer/Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sofia {

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
		std::string vertex2D = R"(
			#version 460 core

			layout(location = 0) in vec4 i_Pos;
			layout(location = 1) in vec4 i_Color;
			layout(location = 2) in vec2 i_UV;
			layout(location = 3) in int i_TID;
			layout(location = 4) in float i_TillingFactor;

			layout(std140, binding = 0) uniform RendererData
			{
				mat4 u_ViewProjMat;
			};

			out Data {
				vec4 color;
				vec2 uv;
				flat int tid;
				flat float tillingFactor;
			} vs_out;

			void main()
			{
				gl_Position = u_ViewProjMat * i_Pos;
				vs_out.color = i_Color;
				vs_out.uv = i_UV;
				vs_out.tid = i_TID;
				vs_out.tillingFactor = i_TillingFactor;
			}
		)";

		std::string fragment2D = R"(
			#version 460 core

			layout(location = 0) out vec4 o_Color;

			in Data {
				vec4 color;
				vec2 uv;
				flat int tid;
				flat float tillingFactor;
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
			}
		)";
		Renderer::GetShaderLibrary().Load("2D", vertex2D, fragment2D);
	}
}
