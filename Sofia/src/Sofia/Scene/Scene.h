#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"
#include "Sofia/Renderer/Camera.h"
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
		SOF_CORE Scene(const std::string& name = "Scene");
		SOF_CORE ~Scene();

		SOF_CORE Entity CreateEntity(const std::string& name = {});
		SOF_CORE Entity CreateEntityWithID(UUID id, const std::string& name = {});
		SOF_CORE void DestroyEntity(Entity entity);

		SOF_CORE Entity SetCameraEntity(); //creates new camera entity
		SOF_CORE Entity SetCameraEntity(Entity cameraEntity);
		SOF_CORE Entity SetCameraEntity(Ref<Camera> camera); //creates new camera entity

		SOF_CORE void OnEvent(Event& e);
		SOF_CORE void OnUpdate(Timestep ts);
		SOF_CORE void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdd(Entity entity, T& component);
	private:
		std::string m_Name;
		uint32_t m_ID = -1;

		entt::registry m_Registry;
		entt::entity m_SceneEntity = entt::null;
		entt::entity m_Camera = entt::null;

		glm::uvec2 m_ViewportSize = { 0u, 0u };
	};
}
