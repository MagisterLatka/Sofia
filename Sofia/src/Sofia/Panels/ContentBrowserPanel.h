#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	inline const std::filesystem::path g_AssetsPath = L"assets";

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;

		void OnUIRender();
	private:
		std::filesystem::path m_CurrDir;

		Ref<Texture2D> m_DirIcon, m_FileIcon;
	};
}
