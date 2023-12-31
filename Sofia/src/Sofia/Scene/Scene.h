#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"
#include "Sofia/Renderer/Camera.h"
#include "Sofia/Scene/EditorCamera.h"
#include "Sofia/Events/Event.h"
#include "Sofia/UUID.h"

#include <entt.hpp>

namespace Sofia {

	class Entity;
	class Scene : public RefCounted
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		struct SceneComponent
		{
			uint32_t SceneID;
		};
	public:
		Scene(const std::string& name = "Scene") noexcept;
		~Scene();

		Entity CreateEntity(const std::string& name = {});
		Entity CreateEntityWithUUID(UUID id, const std::string& name = {});
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);
		Entity GetEntity(UUID id);

		Entity SetCameraEntity(); //creates new camera entity
		Entity SetCameraEntity(Entity cameraEntity);
		Entity SetCameraEntity(Ref<Camera> camera); //creates new camera entity

		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnEvent(Event& e);
		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height) noexcept;

		bool IsRunning() const noexcept { return m_Running; }

		static Ref<Scene> Copy(Ref<Scene> scene);
	private:
		template<typename T>
		void OnComponentAdd(Entity entity, T& component);
	private:
		uint32_t m_ID;
		std::string m_Name;

		entt::registry m_Registry;
		entt::entity m_SceneEntity;
		entt::entity m_Camera = entt::null;
		std::unordered_map<UUID, entt::entity> m_Entities;

		glm::ivec2 m_ViewportSize = { 1600, 900 };
		bool m_Running = false;
	};
}
