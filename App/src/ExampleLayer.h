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
	bool OnKeyPressed(Sofia::KeyPressedEvent& e);
	bool OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e);

	void NewScene();
	void OpenScene();
	void OpenScene(const std::filesystem::path& path);
	void SaveScene();
private:
	Ref<Sofia::RenderPass> m_RenderPass;
	Ref<Sofia::Texture2D> m_Texture;

	Ref<Sofia::Scene> m_Scene;
	Sofia::Entity m_Camera; //TEMP, breaks on loading different scene;
	Scope<Sofia::SceneHierarchyPanel> m_SceneHierarchyPanel;
	Scope<Sofia::ContentBrowserPanel> m_ContentBrowserPanel;
	Sofia::Entity m_HoveredEntity;

	bool m_ViewportHovered = false, m_ViewportFocused = false;
	glm::ivec2 m_ViewportSize = { -1.0f, -1.0f }, m_ViewportPos = { 0.0f, 0.0f };
	int m_GizmoType = -1;
};
