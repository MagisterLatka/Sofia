#include <pch.h>
#include "Scene.h"

#include "Sofia/Scene/ScriptableEntity.h"
#include "Sofia/Scene/Components.h"
#include "Sofia/Renderer/Renderer2D.h"

namespace Sofia {

	static uint32_t s_SceneCounter = 0u;
	Scene::Scene(const std::string& name)
		: m_Name(name), m_ID(++s_SceneCounter)
	{
		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_ID);
		m_Registry.emplace<TagComponent>(m_SceneEntity, "SceneEntity");
	}
	Scene::~Scene()
	{
		m_Registry.clear();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entity = Entity(m_Registry.create(), this);

		if (!name.empty())
			entity.AddComponent<TagComponent>(name);
		else
			entity.AddComponent<TagComponent>("Entity");

		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));

		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_Handle);
	}

	Entity Scene::SetCameraEntity()
	{
		auto entity = CreateEntity("Camera");
		entity.AddComponent<CameraComponent>(Ref<OrthographicCamera>::Create(16.0f / 9.0f));
		m_Camera = entity;
		return entity;
	}
	Entity Scene::SetCameraEntity(Entity cameraEntity)
	{
		m_Camera = cameraEntity;
		return cameraEntity;
	}
	Entity Scene::SetCameraEntity(Ref<Camera> camera)
	{
		auto entity = CreateEntity("Camera");
		entity.AddComponent<CameraComponent>(camera);
		m_Camera = entity;
		return entity;
	}

	void Scene::OnEvent(Event& e)
	{
		m_Registry.view<NativeScriptComponent>().each([&](entt::entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity(entity, this);
				nsc.Instance->OnCreate();
			}
			nsc.Instance->OnEvent(e);
		});
	}
	void Scene::OnUpdate(Timestep ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity(entity, this);
				nsc.Instance->OnCreate();
			}
			nsc.Instance->OnUpdate(ts);
		});
		m_Registry.view<TransformComponent>().each([](entt::entity entity, TransformComponent& tc)
		{
			tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
			glm::quat rotation = glm::quat(tc.Orientation);
			tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
		});

		if (m_Camera != entt::null)
		{
			auto camera = m_Registry.get<CameraComponent>(m_Camera).Camera;
			auto& transform = m_Registry.get<TransformComponent>(m_Camera).Transform;
			Renderer2D::SetViewProjectionMatrix(camera->GetProjectionMatrix() * glm::inverse(transform));

			auto group = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::SubmitQuad(tc, sc.Color, sc.Texture, sc.TillingFactor, (uint32_t)entity);
			}
			Renderer2D::DrawQuads();
		}
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width, height };

		m_Registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity(entity, this);
				nsc.Instance->OnCreate();
			}
			nsc.Instance->OnViewportResize(width, height);
		});
		m_Registry.view<CameraComponent>().each([=](entt::entity entity, CameraComponent& cc)
		{
			if (cc.Camera)
				cc.Camera->SetViewportSize(width, height);
		});
	}

	template<>
	SOF_CORE void Scene::OnComponentAdd<TagComponent>(Entity entity, TagComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<TransformComponent>(Entity entity, TransformComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<SpriteComponent>(Entity entity, SpriteComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}
	template<>
	SOF_CORE void Scene::OnComponentAdd<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
}
