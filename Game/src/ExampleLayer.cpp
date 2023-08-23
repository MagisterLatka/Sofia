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

	auto window = Sofia::Application::Get().GetWindow();
	m_RenderTarget = Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight());
}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		m_RenderTarget->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

	m_RenderTarget->Bind();
	m_RenderTarget->Clear();

	m_Shader->Bind();
	m_InputLayout->Bind();

	Sofia::RenderCommand::DrawIndexed(Sofia::RendererAPI::Topology::Triangles, 6u);

	Sofia::Application::Get().GetWindow()->Clear();
	Sofia::Application::Get().GetWindow()->BindToRender();

	Sofia::Renderer::Render();
}
void ExampleLayer::OnUIRender()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Settings");

	ImGui::Text("Frame time: %.3fms (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	auto& application = Sofia::Application::Get();

	m_ViewportHovered = ImGui::IsWindowHovered();
	m_ViewportFocused = ImGui::IsWindowFocused();
	application.GetImGuiLayer()->BlockEvents(!m_ViewportHovered && !m_ViewportFocused);

	auto viewportSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportSize.x, viewportSize.y };
	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 offset = ImGui::GetWindowContentRegionMin();
	viewportPos.x += offset.x;
	viewportPos.y += offset.y;
	m_ViewportPos = { viewportPos.x - application.GetWindow()->GetXClientPos(), viewportPos.y - application.GetWindow()->GetYClientPos() };

	ImGui::Image(m_RenderTarget->GetRawTexturePointer(), viewportSize);

	ImGui::End();
	ImGui::PopStyleVar();
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
