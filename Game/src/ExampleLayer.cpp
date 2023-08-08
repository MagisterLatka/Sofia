#include <pch.h>
#include <Sofia.h>
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Sofia::Layer("Example layer")
{}
ExampleLayer::~ExampleLayer()
{}

void ExampleLayer::OnAttach()
{}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{

}
void ExampleLayer::OnImGuiRender()
{}
void ExampleLayer::OnEvent(Sofia::Event & e)
{}
