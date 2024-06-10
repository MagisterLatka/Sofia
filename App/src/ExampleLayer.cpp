#include <pch.h>
#include "Sofia.h"
#include "ExampleLayer.h"

#include <Sofia/ImGui/ImGui.h>
#include <Sofia/ImGui/ImGuizmo.h>

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
	m_RenderPass->SetRenderTarget(1u, Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Sofia::RenderTargetFormat::R32_UINT));
	m_RenderPass->SetDepthStencilTarget(Sofia::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Sofia::RenderTargetFormat::Depth32F));

	Sofia::Texture2DProps textureProps;
	textureProps.Filepath = L"assets/textures/checkerboard.png";
	textureProps.Sampling = Sofia::TextureSampling::Point;
	m_Texture = Sofia::Texture2D::Create(textureProps);

	m_Scene = Ref<Sofia::Scene>::Create("App scene");
	m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());
	m_Camera = m_Scene->SetCameraEntity();
	m_Camera.GetComponent<Sofia::CameraComponent>().Camera.As<Sofia::OrthographicCamera>()->SetSize(2.0f);
	m_Camera.AddComponent<Sofia::NativeScriptComponent>().Bind<CameraController>();

	m_Scene->CreateEntity("Textured quad").AddComponent<Sofia::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_Texture);

	m_SceneHierarchyPanel = CreateScope<Sofia::SceneHierarchyPanel>(m_Scene);
	m_ContentBrowserPanel = CreateScope<Sofia::ContentBrowserPanel>();
}
void ExampleLayer::OnDetach()
{
	m_ContentBrowserPanel.reset();
	m_SceneHierarchyPanel.reset();
	m_Scene.Reset();
	m_RenderPass.Reset();
	m_Texture.Reset();
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

	m_Scene->OnUpdate(ts);

	glm::vec2 mousePos = Sofia::Input::GetMousePos();
	mousePos.x -= m_ViewportPos.x;
	mousePos.y -= m_ViewportPos.y;
	if (mousePos.x > 0.0f && mousePos.y >= 0.0f && mousePos.x < m_ViewportSize.x && mousePos.y < m_ViewportSize.y)
	{
		static uint32_t data = 0;
		m_RenderPass->GetRenderTarget(1u)->ReadPixel(&data, (uint32_t)mousePos.x, (uint32_t)mousePos.y);
		Sofia::Renderer::Submit([this]()
		{
			m_HoveredEntity = data == 0 || data == -1 ? Sofia::Entity() : Sofia::Entity((entt::entity)data, m_Scene.Raw());
		});
	}

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
	ImGui::Text("Circle count: %d", Sofia::Renderer2D::GetStats().CircleCount);
	std::string name = "None";
	if (m_HoveredEntity)
		name = m_HoveredEntity.GetComponent<Sofia::TagComponent>().Tag;
	ImGui::Text("Hovered entity: %s", name.c_str());

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

	ImVec2 uv0 = { 0, 0 }, uv1 = { 1, 1 };
	if (Sofia::RendererAPI::GetAPI() == Sofia::RendererAPI::API::OpenGL)
		uv0 = { 0, 1 }, uv1 = { 1, 0 };
	ImGui::Image(m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize, uv0, uv1);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			OpenScene(Sofia::g_AssetsPath / path);
		}
		ImGui::EndDragDropTarget();
	}

	if (Sofia::Entity selected = m_SceneHierarchyPanel->GetSelected(); m_GizmoType != -1 && selected)
	{
		const bool snap = Sofia::Input::IsKeyPressed(Sofia::KeyCode::LeftControl);
		const float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : m_GizmoType == ImGuizmo::OPERATION::SCALE ? 0.5f : 0.1f;
		const float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

		const glm::mat4 viewMat = glm::inverse(m_Camera.GetTransform());
		const glm::mat4& projMat = m_Camera.GetComponent<Sofia::CameraComponent>().Camera->GetProjectionMatrix();
		glm::mat4 transform = selected.GetTransform();

		ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
			snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			auto& tc = selected.GetTransformComponent();
			glm::vec3 pos, size, skew;
			glm::quat orientation;
			glm::vec4 perspective;
			glm::decompose(transform, size, orientation, pos, skew, perspective);
			tc.Position = pos;
			tc.Orientation += glm::eulerAngles(orientation) - tc.Orientation;
			tc.Size = size;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	m_SceneHierarchyPanel->OnUIRender();
	m_ContentBrowserPanel->OnUIRender();
}

void ExampleLayer::OnEvent(Sofia::Event& e)
{
	Sofia::Dispatcher dispatcher(e);
	dispatcher.Dispatch<Sofia::MouseButtonPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
	dispatcher.Dispatch<Sofia::KeyPressedEvent>(SOF_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));

	m_Scene->OnEvent(e);
}
bool ExampleLayer::OnKeyPressed(Sofia::KeyPressedEvent& e)
{
	bool shift = Sofia::Input::IsKeyPressed(Sofia::KeyCode::LeftShift) || Sofia::Input::IsKeyPressed(Sofia::KeyCode::RightShift);
	bool control = Sofia::Input::IsKeyPressed(Sofia::KeyCode::LeftControl) || Sofia::Input::IsKeyPressed(Sofia::KeyCode::RightControl);

	switch (e.GetKeyCode())
	{
	case Sofia::KeyCode::N:
		if (control)
			NewScene();
		break;
	case Sofia::KeyCode::O:
		if (control)
			OpenScene();
		break;
	case Sofia::KeyCode::S:
		if (control && shift)
			SaveScene();
		break;
	case Sofia::KeyCode::Q:
		if (!ImGuizmo::IsUsing())
			m_GizmoType = -1;
		break;
	case Sofia::KeyCode::W:
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		break;
	case Sofia::KeyCode::E:
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		break;
	case Sofia::KeyCode::R:
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
		break;
	}

	return false;
}
bool ExampleLayer::OnMouseButtonPressed(Sofia::MouseButtonPressedEvent& e)
{
	switch (e.GetButton())
	{
	case Sofia::MouseCode::ButtonLeft:
		if (m_ViewportHovered && !ImGuizmo::IsOver())
			m_SceneHierarchyPanel->SetSelected(m_HoveredEntity);
	}
	return false;
}

void ExampleLayer::NewScene()
{
	m_Scene = Ref<Sofia::Scene>::Create();
	m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	m_SceneHierarchyPanel->SetScene(m_Scene);
}
void ExampleLayer::OpenScene()
{
	std::filesystem::path filepath = Sofia::FileProcessing::ChooseFileToOpenFrom(L"Saba scene (*.scene)\0*.scene\0");
	if (filepath.empty())
		return;
	OpenScene(filepath);
}
void ExampleLayer::OpenScene(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) || path.extension() != L".scene")
		return;

	m_Scene = Ref<Sofia::Scene>::Create();
	m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	m_SceneHierarchyPanel->SetScene(m_Scene);

	Sofia::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(path);
}
void ExampleLayer::SaveScene()
{
	std::filesystem::path filepath = Sofia::FileProcessing::ChooseFileToSaveTo(L"Saba scene (*.scene)\0*.scene\0");
	if (filepath.empty())
		return;

	Sofia::SceneSerializer serializer(m_Scene);
	serializer.Serialize(filepath);
}
