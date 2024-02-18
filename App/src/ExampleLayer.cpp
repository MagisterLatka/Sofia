#include <pch.h>
#include "Sofia.h"
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
{

}
ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{

}
void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->Clear();

	glm::vec2 pos = Sofia::Input::GetMousePos();
	std::ostringstream oss;
	oss << pos.x << ", " << pos.y;
	m_Title = oss.str();
}
void ExampleLayer::OnUIRender()
{

}

void ExampleLayer::OnEvent(Sofia::Event& e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}
bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	if (e.GetButton() == Sofia::MouseCode::ButtonLeft)
	{
		Sofia::Application::Get().GetWindow()->SetTitle(m_Title);
		SOF_TRACE("Set title to {0}", m_Title);
	}
	return false;

}
