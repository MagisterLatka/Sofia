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

	auto window = Sofia::Application::Get().GetWindow();
	m_RenderTarget = Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight());

	ImGui::SetCurrentContext(Sofia::Application::Get().GetImGuiLayer()->GetContext());
}
void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->BindWindow();

	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		m_RenderTarget->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

	m_RenderTarget->Bind();
	m_RenderTarget->Clear();

	m_Shader->Bind();
	m_InputLayout->Bind();

	Sofia::RenderCommand::Draw(Sofia::RendererAPI::Topology::Triangles, 3u);

	Sofia::Application::Get().GetWindow()->BindToRender();
	Sofia::Application::Get().GetWindow()->Clear();
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

	if (Sofia::RendererAPI::GetAPI() == Sofia::RendererAPI::API::OpenGL)
		ImGui::Image(m_RenderTarget->GetRawTexturePointer(), viewportSize, { 0, 1 }, { 1, 0 });
	else
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
