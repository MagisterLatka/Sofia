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
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f
	};
	Sofia::BufferLayout layout = {
		{ "Position", Sofia::BufferLayoutElementDataType::Float3 },
		{ "Color", Sofia::BufferLayoutElementDataType::Float4 }
	};
	Ref<Sofia::VertexBuffer> vbo = Sofia::VertexBuffer::Create(layout, vertices, sizeof(vertices));
	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	Ref<Sofia::IndexBuffer> ibo = Sofia::IndexBuffer::Create(indices, sizeof(indices), Sofia::BufferUsage::Immutable);
	m_InputLayout = Sofia::InputLayout::Create({ vbo }, m_Shader, ibo);

	Sofia::UniformBuffer<sizeof(glm::vec4), 1u> buffer;
	buffer.Push("Tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ConstantBuffer = Sofia::ConstantBuffer::Create(Sofia::BufferShaderBinding::Fragment, buffer);
}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	static float counter = 0.0f;
	static glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
	if (Sofia::Input::IsMouseButtonPressed(SOF_MOUSE_BUTTON_LEFT))
	{
		counter += 0.05f;
		color.x = glm::cos(counter) * 0.5f + 0.5f;
		color.z = glm::cos(counter) * 0.5f + 0.5f;
		m_InputLayout->GetVertexBuffer()->SetData(glm::value_ptr(color), sizeof(glm::vec4), 24 * sizeof(float));
	}
	static glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (Sofia::Input::IsMouseButtonPressed(SOF_MOUSE_BUTTON_RIGHT))
	{
		counter += 0.05f;
		tint = glm::vec4(glm::vec3(glm::cos(counter) * 0.5f + 0.5f), 1.0f);
		m_ConstantBuffer->SetData(glm::value_ptr(tint), sizeof(glm::vec4));
	}

	Sofia::Application::Get().GetWindow()->Clear();
	Sofia::Application::Get().GetWindow()->BindToRender();
	
	m_Shader->Bind();
	m_InputLayout->Bind();
	m_ConstantBuffer->Bind();

	Sofia::RenderCommand::DrawIndexed(Sofia::RendererAPI::Topology::Triangles, 6u);
	Sofia::Renderer::Render();
}
void ExampleLayer::OnImGuiRender()
{}
void ExampleLayer::OnEvent(Sofia::Event& e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}

bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	return false;
}
