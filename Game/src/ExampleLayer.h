#pragma once

#include <Sofia/Panels/SceneHierarchyPanel.h>
#include <Sofia/Panels/ContentBrowserPanel.h>

class ExampleLayer : public Sofia::Layer
{
	friend Sofia::Application* Sofia::CreateApplication();
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Sofia::Timestep ts) override;
	virtual void OnUIRender() override;
	virtual void OnEvent(Sofia::Event& e) override;
private:
	bool OnMouseButttonPressed(Sofia::MouseButtonPressedEvent& e);
	bool OnKeyPressed(Sofia::KeyPressedEvent& e);

	void NewScene();
	void OpenScene();
	void OpenScene(const std::filesystem::path& path);
	void SaveScene();
	void SaveSceneAs();
	void OnScenePlay();
	void OnSceneStop();
	void OnDuplicateEntity();
private:
	Ref<Sofia::RenderPass> m_RenderPass;

	Scope<Sofia::SceneHierarchyPanel> m_SceneHierarchyPanel;
	Scope<Sofia::ContentBrowserPanel> m_ContentBrowserPanel;

	enum class SceneState
	{
		Edit = 0, Play = 1
	};
	Ref<Sofia::Scene> m_ActiveScene, m_EditorScene;
	Ref<Sofia::Texture2D> m_PlayButton, m_StopButton;
	SceneState m_SceneState = SceneState::Edit;
	std::filesystem::path m_EditorScenePath;
	Sofia::EditorCamera m_EditorCamera;
	Sofia::Entity m_HoveredEntity;
	Sofia::Entity m_Camera;

	bool m_ViewportHovered = false, m_ViewportFocused = false;
	glm::ivec2 m_ViewportSize = { 0, 0 }, m_ViewportPos = { 0, 0 };
	int m_GuizmoType = -1;
};
