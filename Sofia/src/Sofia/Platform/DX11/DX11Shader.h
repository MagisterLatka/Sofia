#pragma once

#include "Sofia/Renderer/Shader.h"
#include <d3d11.h>

namespace Sofia {

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		DX11Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName);
		virtual ~DX11Shader() = default;

		virtual void Reload() override;

		virtual void Bind() const noexcept override;

		virtual const std::string& GetName() const override { return m_Name; };

		virtual void AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept override;

		ComPtr<ID3DBlob> GetVertexSource() const noexcept { return m_VSource; }
	private:
		ComPtr<ID3D11VertexShader> m_VShader;
		ComPtr<ID3D11PixelShader> m_FShader;
		ComPtr<ID3DBlob> m_VSource;
		std::filesystem::path m_VPath, m_FPath;
		std::string m_VString, m_FString;
		bool m_UseFilepath = false;
		std::string m_Name;
		bool m_Loaded = false;
		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallback;
	};
}
