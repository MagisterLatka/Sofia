#pragma once

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
	std::string m_Title;
};
