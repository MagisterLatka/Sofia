#pragma once

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
	bool OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e);
private:
	Ref<Sofia::RenderPass> m_RenderPass;
	Ref<Sofia::Texture2D> m_Texture;

	bool m_ViewportHovered = false, m_ViewportFocused = false;
	glm::vec2 m_ViewportSize = { -1.0f, -1.0f }, m_ViewportPos;
};
