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
		return CreateEntityWithID(UUID(), name);
	}
	Entity Scene::CreateEntityWithID(UUID id, const std::string& name)
	{
		auto entity = Entity(m_Registry.create(), this);
		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		auto& tag = entity.AddComponent<TagComponent>().Tag;
		tag = name.empty() ? "Entity" : name;

		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_Handle);
	}
	template<typename T>
	static void CopyComponent(Entity src, Entity dst)
	{
		if (src.HasComponent<T>())
			dst.AddReplaceComponent<T>(src.GetComponent<T>());
	}
	void Scene::DuplicateEntity(Entity entity)
	{
		Entity e = CreateEntity(entity.GetComponent<TagComponent>().Tag);
		CopyComponent<TransformComponent>(entity, e);
		CopyComponent<SpriteComponent>(entity, e);
		CopyComponent<CircleComponent>(entity, e);
		CopyComponent<CameraComponent>(entity, e);
		CopyComponent<NativeScriptComponent>(entity, e);
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
	void Scene::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
	{
		//update transforms
		m_Registry.view<TransformComponent>().each([](entt::entity entity, TransformComponent& tc)
		{
			tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
			glm::quat rotation = glm::quat(tc.Orientation);
			tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
		});

		//render 2D
		Renderer2D::SetViewProjectionMatrix(camera.GetProjectionViewMatrix());
		auto group = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			auto [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::SubmitQuad(tc, sc.Color, sc.Texture, sc.TillingFactor, (uint32_t)entity);
		}
		Renderer2D::DrawQuads();

		auto group1 = m_Registry.group<CircleComponent>(entt::get<TransformComponent>);
		for (auto entity : group1)
		{
			auto [tc, cc] = group1.get<TransformComponent, CircleComponent>(entity);
			Renderer2D::SubmitCircle(tc, cc.Color, cc.Thickness, cc.Fade, (uint32_t)entity);
		}
		Renderer2D::DrawCircles();
	}
	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//native scripts
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

		//update transforms
		m_Registry.view<TransformComponent>().each([](entt::entity entity, TransformComponent& tc)
		{
			tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
			glm::quat rotation = glm::quat(tc.Orientation);
			tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
		});

		//render 2D
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

			auto group1 = m_Registry.group<CircleComponent>(entt::get<TransformComponent>);
			for (auto entity : group1)
			{
				auto [tc, cc] = group1.get<TransformComponent, CircleComponent>(entity);
				Renderer2D::SubmitCircle(tc, cc.Color, cc.Thickness, cc.Fade, (uint32_t)entity);
			}
			Renderer2D::DrawCircles();
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

	template<typename T>
	static void CopyComponent(entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& entities)
	{
		auto view = src.view<T>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;
			SOF_CORE_ASSERT(entities.find(uuid) != entities.end());
			entt::entity id = entities.at(uuid);
			T& component = src.get<T>(e);
			dst.emplace_or_replace<T>(id, component);
		}
	}
	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> newScene = Ref<Scene>::Create(scene->m_Name);
		newScene->m_ViewportSize = scene->m_ViewportSize;

		std::unordered_map<UUID, entt::entity> entities;
		auto view = scene->m_Registry.view<IDComponent>();
		for (auto e = view.rbegin(); e != view.rend(); ++e)
		{
			UUID uuid = scene->m_Registry.get<IDComponent>(*e).ID;
			const std::string& name = scene->m_Registry.get<TagComponent>(*e).Tag;
			Entity entity = newScene->CreateEntityWithID(uuid, name);
			entities[uuid] = entity;
		}

		CopyComponent<TransformComponent>(scene->m_Registry, newScene->m_Registry, entities);
		CopyComponent<SpriteComponent>(scene->m_Registry, newScene->m_Registry, entities);
		CopyComponent<CircleComponent>(scene->m_Registry, newScene->m_Registry, entities);
		CopyComponent<CameraComponent>(scene->m_Registry, newScene->m_Registry, entities);
		CopyComponent<NativeScriptComponent>(scene->m_Registry, newScene->m_Registry, entities);

		newScene->m_Camera = entities.at(scene->m_Registry.get<IDComponent>(scene->m_Camera).ID);

		return newScene;
	}

	template<>
	SOF_CORE void Scene::OnComponentAdd<IDComponent>(Entity entity, IDComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<TagComponent>(Entity entity, TagComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<TransformComponent>(Entity entity, TransformComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<SpriteComponent>(Entity entity, SpriteComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<CircleComponent>(Entity entity, CircleComponent& component) {}
	template<>
	SOF_CORE void Scene::OnComponentAdd<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}
	template<>
	SOF_CORE void Scene::OnComponentAdd<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
}
