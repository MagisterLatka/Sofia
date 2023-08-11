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
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Sofia::Event& e) override;
private:
	bool OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e);
private:
	Ref<Sofia::Shader> m_Shader;
	Ref<Sofia::InputLayout> m_InputLayout;
};
