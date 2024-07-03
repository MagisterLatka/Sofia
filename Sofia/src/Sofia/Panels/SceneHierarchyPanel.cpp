#include <pch.h>
#include "SceneHierarchyPanel.h"

#include "Sofia/ImGui/ImGui.h"
#include "Sofia/Panels/ContentBrowserPanel.h"

namespace Sofia {

	void SceneHierarchyPanel::SetScene(Ref<Scene> scene) noexcept
	{
		m_Scene = scene;
		m_Selected = {};
	}
	void SceneHierarchyPanel::SetSelected(Entity entity) noexcept
	{
		if (!entity)
			m_Selected = {};
		else if (entity.GetScene() == m_Scene.Raw())
			m_Selected = entity;
	}

	void SceneHierarchyPanel::OnUIRender()
	{
		ImGui::Begin("Scene hierarchy");

		if (m_Scene)
		{
			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, m_Scene->m_Name.c_str(), sizeof(buffer) - 1);
			if (ImGui::InputText("Scene name###Tag", buffer, sizeof(buffer)))
				m_Scene->m_Name = buffer;

			m_Scene->m_Registry.view<entt::entity>().each([&](entt::entity id)
			{
				Entity entity(id, m_Scene.Raw());
				DrawEntityNode(entity);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_Selected = {};

			if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
			{
				if (ImGui::MenuItem("Create entity"))
					m_Selected = m_Scene->CreateEntity();

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");

		DrawComponents(m_Selected);

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		if (entity.HasComponent<Scene::SceneComponent>())
			return;

		auto& tag = entity.GetComponent<TagComponent>().Tag;

		std::string nameID = tag + "###" + std::to_string((uint32_t)entity);
		if (ImGui::Selectable(nameID.c_str(), m_Selected == entity, ImGuiSelectableFlags_AllowDoubleClick))
			m_Selected = entity;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy entity"))
			{
				m_Scene->DestroyEntity(entity);
				if (m_Selected == entity)
					m_Selected = {};
			}

			ImGui::EndPopup();
		}
	}
	template<typename T, typename F>
	static void DrawComponent(Ref<Scene> scene, const std::string& name, Entity entity, F func, bool removable = true)
	{
		if (!entity.HasComponent<T>())
			return;

		auto& component = entity.GetComponent<T>();
		ImVec2 regionAvail = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding);
		ImGui::PopStyleVar();

		ImGui::SameLine(regionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button("...", { lineHeight, lineHeight }))
			ImGui::OpenPopup("Settings");

		bool removeComponent = false;
		if (removable)
		{
			if (ImGui::BeginPopup("Settings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}
		}

		if (opened)
		{
			func(component, scene);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (!entity) return;

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);
			if (ImGui::InputText("###Tag", buffer, sizeof(buffer)))
				tag = buffer;
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("add");

		if (ImGui::BeginPopup("add"))
		{
			if (!entity.HasComponent<CameraComponent>() && !entity.HasComponent<SpriteComponent>() && !entity.HasComponent<CircleComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					entity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!entity.HasComponent<SpriteComponent>() && !entity.HasComponent<CameraComponent>() && !entity.HasComponent<CircleComponent>())
			{
				if (ImGui::MenuItem("Sprite"))
				{
					entity.AddComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!entity.HasComponent<CircleComponent>() && !entity.HasComponent<SpriteComponent>() && !entity.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Circle"))
				{
					entity.AddComponent<CircleComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		constexpr float columnWidth = 85.0f;
		DrawComponent<TransformComponent>(m_Scene, "Transform component", entity, [](TransformComponent& component, Ref<Scene> scene)
		{
			Sofia::UI::DragFloat3("Pos", component.Position, 0.0f, 0.0f, 0.0f, 0.1f, columnWidth);

			glm::vec3 orientationDegrees = glm::degrees(component.Orientation);
			Sofia::UI::DragFloat3("Orientation", orientationDegrees, 0.0f, 0.0f, 0.0f, 1.0f, columnWidth);
			component.Orientation = glm::radians(orientationDegrees);

			Sofia::UI::DragFloat3("Scale", component.Size, 1.0f, 0.01f, 10.0f, 0.01f, columnWidth);
		}, false);
		DrawComponent<SpriteComponent>(m_Scene, "Sprite component", entity, [](SpriteComponent& component, Ref<Scene> scene)
		{
			Sofia::UI::ColorEdit4("Color", component.Color, columnWidth);

			ImGui::Button("Texture", ImVec2(ImGui::GetColumnWidth(), 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					Texture2DProps props;
					props.Filepath = g_AssetsPath / path;
					Ref<Texture2D> texture = Texture2D::Create(props);
					if (texture->IsLoaded())
						component.Texture = texture;
					else
						SOF_CORE_WARN("Loading texture {0} failed", props.Filepath.string());
				}
				ImGui::EndDragDropTarget();
			}

			Sofia::UI::DragFloat("Tilling factor", component.TillingFactor, 1.0f, 0.01f, 10.0f, 0.01f, columnWidth);
		});
		DrawComponent<CircleComponent>(m_Scene, "Circle component", entity, [](CircleComponent& component, Ref<Scene> scene)
		{
			Sofia::UI::ColorEdit4("Color", component.Color, columnWidth);
			Sofia::UI::DragFloat("Thickness", component.Thickness, 1.0f, 0.01f, 1.0f, 0.01f, columnWidth);
			Sofia::UI::DragFloat("Fade", component.Fade, 0.005f, 0.001f, 1.0f, 0.001f, columnWidth);
		});
		DrawComponent<CameraComponent>(m_Scene, "Camera component", entity, [](CameraComponent& component, Ref<Scene> scene)
		{
			if (!component.Camera)
				return;

			if (component.Camera.CanConvert<OrthographicCamera>())
			{
				auto camera = component.Camera.As<OrthographicCamera>();

				bool changed = false;
				changed |= Sofia::UI::DragFloat("Aspect ratio", camera->m_AspectRatio, 16.0f / 9.0f, 0.01f, 5.0f, 0.01f, columnWidth);
				changed |= Sofia::UI::DragFloat("Size", camera->m_Size, 1.0f, 0.5f, 20.0f, 0.1f, columnWidth);
				changed |= Sofia::UI::DragFloat("Near clip", camera->m_NearClip, -1.0f, -10.0f, -1.0f, 0.1f, columnWidth);
				changed |= Sofia::UI::DragFloat("Far clip", camera->m_FarClip, 1.0f, 1.0f, 10.0f, 0.1f, columnWidth);

				if (changed) camera->Recalc();
			}
		});
	}
}
