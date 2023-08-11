#include <pch.h>
#include <Sofia.h>
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
	: Sofia::Layer("Example layer")
{}
ExampleLayer::~ExampleLayer()
{}

void ExampleLayer::OnAttach()
{
	m_Shader = Sofia::Shader::Create(L"assets/shaders/basic.vert.cso", L"assets/shaders/basic.frag.cso");

	float vertices[] = {
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f
	};
	Sofia::BufferLayout layout = {
		{ "Position", Sofia::BufferLayoutElementDataType::Float2 }
	};
	Ref<Sofia::VertexBuffer> vbo = Sofia::VertexBuffer::Create(layout, vertices, sizeof(vertices));
	m_InputLayout = Sofia::InputLayout::Create({ vbo }, m_Shader);
}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->Clear();
	Sofia::Application::Get().GetWindow()->BindToRender();
	
	m_Shader->Bind();
	m_InputLayout->Bind();

	Sofia::RenderCommand::Draw(Sofia::RendererAPI::Topology::Triangles, 3u);
	Sofia::Renderer::Render();
}
void ExampleLayer::OnImGuiRender()
{}
void ExampleLayer::OnEvent(Sofia::Event & e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}

bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	return false;
}
