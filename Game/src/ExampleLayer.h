#pragma once

#include <d3d11.h>

class ExampleLayer : public Sofia::Layer
{
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
	Ref<Sofia::RenderTarget> m_RenderTarget;
	Ref<Sofia::Texture2D> m_Texture;

	bool m_ViewportHovered = false, m_ViewportFocused = false;
	glm::vec2 m_ViewportSize, m_ViewportPos;
};
