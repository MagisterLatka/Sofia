#include <pch.h>
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Sofia {

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDir(g_AssetsPath)
	{
		Texture2DProps props;
		props.Filepath = g_AssetsPath / L"DirectoryIcon.png";
		props.Sampling = TextureSampling::Point;
		m_DirIcon = Texture2D::Create(props);
		props.Filepath = g_AssetsPath / L"FileIcon.png";
		m_FileIcon = Texture2D::Create(props);
	}

	void ContentBrowserPanel::OnUIRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDir != g_AssetsPath)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDir = m_CurrentDir.parent_path();
			}
		}

		static float padding = 16.0f, iconSize = 114.0f;
		float cellSize = iconSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& dir : std::filesystem::directory_iterator(m_CurrentDir))
		{
			const auto& path = dir.path();
			auto relativePath = std::filesystem::relative(path, g_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = dir.is_directory() ? m_DirIcon : m_FileIcon;
			ImGui::PushID(filenameString.c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(icon->GetRawPointer(), { cellSize, cellSize });

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dir.is_directory())
					m_CurrentDir /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Icon size", &iconSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}
