#include <pch.h>
#include "Sofia.h"
#include "ExampleLayer.h"

#include <imgui.h>

class CameraController : public Sofia::ScriptableEntity
{
public:
	CameraController() = default;
	~CameraController() = default;

	virtual void OnCreate() override
	{
		m_Camera = GetComponent<Sofia::CameraComponent>().Camera.As<Sofia::OrthographicCamera>();
	}
	virtual void OnDestroy() override
	{
		m_Camera.Reset();
	}
	virtual void OnEvent(Sofia::Event& e) override
	{
		Sofia::Dispatcher dispatcher(e);
		dispatcher.Dispatch<Sofia::MouseScrolledEvent>(SOF_BIND_EVENT_FN(CameraController::OnMouseScroll));
	}
	bool OnMouseScroll(Sofia::MouseScrolledEvent& e)
	{
		m_Camera->SetSize(m_Camera->GetSize() * (1 - e.GetYOffset() * 0.1f));
		return false;
	}
	virtual void OnUpdate(Sofia::Timestep ts) override
	{
		const float cameraSize = m_Camera->GetSize();
		float deltaSpeed = p_Speed * (float)ts;
		if (cameraSize > 1.0f)
			deltaSpeed *= glm::pow(1.01f, cameraSize) - 0.01f;

		glm::vec3 delta = glm::vec3(0.0f);
		if (Sofia::Input::IsKeyPressed(SOF_KEY_UP))
			delta.y = deltaSpeed;
		else if (Sofia::Input::IsKeyPressed(SOF_KEY_DOWN))
			delta.y = -deltaSpeed;

		if (Sofia::Input::IsKeyPressed(SOF_KEY_LEFT))
			delta.x = -deltaSpeed;
		else if (Sofia::Input::IsKeyPressed(SOF_KEY_RIGHT))
			delta.x = deltaSpeed;

		GetTransformComponent().Position += delta;
	}
	virtual void OnViewportResize(uint32_t width, uint32_t height) override
	{
		m_Camera->SetViewportSize(width, height);
	}
public:
	float p_Speed = 5.0f;
private:
	Ref<Sofia::OrthographicCamera> m_Camera;
};

ExampleLayer::ExampleLayer()
{

}
ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{
	auto window = Sofia::Application::Get().GetWindow();
	m_RenderPass = Sofia::RenderPass::Create();
	m_RenderPass->SetRenderTarget(0u, Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight()));

	Sofia::Texture2DProps textureProps;
	textureProps.Filepath = L"assets/textures/checkerboard.png";
	textureProps.Sampling = Sofia::TextureSampling::Point;
	m_Texture = Sofia::Texture2D::Create(textureProps);

	m_Scene = Ref<Sofia::Scene>::Create("App scene");
	m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());
	m_Camera = m_Scene->SetCameraEntity();
	m_Camera.AddComponent<Sofia::NativeScriptComponent>().Bind<CameraController>();

	m_Quad = m_Scene->CreateEntity("Textured quad");
	m_Quad.AddComponent<Sofia::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_Texture);

	ImGui::SetCurrentContext(Sofia::Application::Get().GetImGuiLayer()->GetContext());
}
void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Sofia::Timestep ts)
{
	Sofia::Application::Get().GetWindow()->BindWindow();

	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (m_ViewportSize.x != m_RenderPass->GetWidth() || m_ViewportSize.y != m_RenderPass->GetHeight()))
	{
		m_RenderPass->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	m_RenderPass->Bind();
	m_RenderPass->Clear();
	Sofia::Renderer2D::ResetStats();

	static float time = 0.0f;
	time += (float)ts;
	time = glm::mod(time, glm::two_pi<float>());
	m_Quad.GetTransformComponent().Orientation.z = time;

	m_Scene->OnUpdate(ts);

	Sofia::Application::Get().GetWindow()->BindToRender();
	Sofia::Application::Get().GetWindow()->Clear();
}
void ExampleLayer::OnUIRender()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Settings");

	ImGui::Text("Frame time: %.3fms (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("Draw calls: %d", Sofia::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad count: %d", Sofia::Renderer2D::GetStats().QuadCount);
	auto camera = m_Camera.GetComponent<Sofia::CameraComponent>().Camera.As<Sofia::OrthographicCamera>();
	float cameraSize = camera->GetSize();
	if (ImGui::DragFloat("Camera size", &cameraSize, 0.025f, 0.1f, 10.0f))
		camera->SetSize(cameraSize);

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
		ImGui::Image(m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize, {0, 1}, {1, 0});
	else
		ImGui::Image(m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize);

	ImGui::End();
	ImGui::PopStyleVar();
}

void ExampleLayer::OnEvent(Sofia::Event& e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));

	m_Scene->OnEvent(e);
}
bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	return false;
}
