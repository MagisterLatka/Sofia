#pragma once

#include "Sofia/Renderer/Shader.h"
#include <d3d11.h>

namespace Sofia {

	class DX11Shader : public Shader
	{
	public:
		SOF_CORE DX11Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		SOF_CORE DX11Shader(const std::filesystem::path& path);
		SOF_CORE DX11Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName);
		SOF_CORE ~DX11Shader() = default;

		SOF_CORE virtual void Reload() override;
		SOF_CORE virtual void Reload(const std::string& vertexSource, const std::string& fragmentSource) override;

		SOF_CORE virtual void Bind() const noexcept override;

		SOF_CORE virtual const std::string& GetName() const override { return m_Name; };

		SOF_CORE virtual void AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept override;

		SOF_CORE ComPtr<ID3DBlob> GetVertexSource() const noexcept { return m_VSource; }
	private:
		SOF_CORE void PreProcess();
		SOF_CORE void CompileFromSource();
	private:
		ComPtr<ID3D11VertexShader> m_VShader;
		ComPtr<ID3D11PixelShader> m_FShader;
		ComPtr<ID3DBlob> m_VSource;
		std::filesystem::path m_VPath, m_FPath;
		std::string m_Name, m_VsSource, m_FsSource;
		bool m_Loaded = false, m_UseFilepath = false;
		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallback;
	};
}
