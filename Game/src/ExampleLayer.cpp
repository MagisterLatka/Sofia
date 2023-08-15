#include <pch.h>
#include <Sofia.h>
#include "ExampleLayer.h"

#include <imgui.h>

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
}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->Clear();
	Sofia::Application::Get().GetWindow()->BindToRender();
	
	m_Shader->Bind();
	m_InputLayout->Bind();

	Sofia::RenderCommand::DrawIndexed(Sofia::RendererAPI::Topology::Triangles, 6u);
	Sofia::Renderer::Render();
}
void ExampleLayer::OnUIRender()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Settings");

	ImGui::Text("Frame time: %.3fms (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();
}
void ExampleLayer::OnEvent(Sofia::Event& e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}

bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	return false;
}
