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
	auto window = Sofia::Application::Get().GetWindow();
	m_RenderPass = Sofia::RenderPass::Create();
	m_RenderPass->SetRenderTarget(0u, Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight()));
	m_RenderPass->SetDepthStencilTarget(Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Sofia::RenderTargetFormat::Depth32F));

	Sofia::Texture2DProps textureProps;
	textureProps.Filepath = L"assets/textures/checkerboard.png";
	textureProps.Sampling = Sofia::TextureSampling::Point;
	m_Texture = Sofia::Texture2D::Create(textureProps);

	float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
	Sofia::Renderer2D::SetViewProjectionMatrix(glm::transpose(glm::orthoLH_ZO(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f)));
	Sofia::RenderCommand::SetBlendOptions(true, Sofia::RendererAPI::BlendOption::SourceAlpha, Sofia::RendererAPI::BlendOption::SourceAlphaInvert);
}
void ExampleLayer::OnDetach()
{}
void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
	{
		m_RenderPass->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		float aspectRatio = m_ViewportSize.x / m_ViewportSize.y;
		Sofia::Renderer2D::SetViewProjectionMatrix(glm::transpose(glm::orthoLH_ZO(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f)));
	}

	m_RenderPass->Bind();
	m_RenderPass->Clear();

	static float time = 0.0f;
	time += (float)ts;
	time = glm::mod(time, glm::two_pi<float>());
	Sofia::Renderer2D::SubmitQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, time, { 1.0f, 1.0f, 1.0f, 1.0f }, m_Texture);
	Sofia::Renderer2D::SubmitQuad({ 0.0f, 0.0f, m_ZPos }, { 1.0f, 1.0f }, 0.0f, { 1.0f, 0.0f, 0.0f, 0.5f });
	Sofia::Renderer2D::Draw();

	Sofia::Application::Get().GetWindow()->Clear();
	Sofia::Application::Get().GetWindow()->BindToRender();

	Sofia::Renderer::Render();
}
void ExampleLayer::OnUIRender()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Settings");

	ImGui::Text("Frame time: %.3fms (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::SliderFloat("ZPos", &m_ZPos, -0.99f, 0.99f);

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

	ImGui::Image(m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize);

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
