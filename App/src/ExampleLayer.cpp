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
	m_Time += ts;
	if (m_Time > 1.0)
	{
		SOF_TRACE("1 sec elapsed");
		m_Time -= 1.0;
	}
}
void ExampleLayer::OnUIRender()
{

}
void ExampleLayer::OnEvent(Sofia::Event& e)
{

}
