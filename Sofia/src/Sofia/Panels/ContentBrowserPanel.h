#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	inline const std::filesystem::path g_AssetsPath = L"assets";

	class ContentBrowserPanel
	{
	public:
		SOF_CORE ContentBrowserPanel();
		SOF_CORE ~ContentBrowserPanel() = default;

		SOF_CORE void OnUIRender();
	private:
		std::filesystem::path m_CurrentDir;
		Ref<Texture2D> m_DirIcon, m_FileIcon;
	};
}
