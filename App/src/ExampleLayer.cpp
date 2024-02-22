#include <pch.h>
#include "Sofia.h"
#include "ExampleLayer.h"

#include <imgui.h>

ExampleLayer::ExampleLayer()
{

}
ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{
	//Later some basic shaders will be defined in engine, other shaders are up to the user to be saved in correct file
	if (Sofia::RendererAPI::GetAPI() == Sofia::RendererAPI::API::DX11)
		m_Shader = Sofia::Shader::Create(L"assets/shaders/basic.vert.cso", L"assets/shaders/basic.frag.cso");
	else
		m_Shader = Sofia::Shader::Create(L"assets/shaders/basic.shader");

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

	ImGui::SetCurrentContext(Sofia::Application::Get().GetImGuiLayer()->GetContext());
}
void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->BindToRender();
	Sofia::Application::Get().GetWindow()->Clear();

	m_Shader->Bind();
	m_InputLayout->Bind();

	Sofia::RenderCommand::Draw(Sofia::RendererAPI::Topology::Triangles, 3u);
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
