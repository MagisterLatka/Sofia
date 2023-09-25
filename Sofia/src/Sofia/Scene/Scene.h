#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"
#include "Sofia/Renderer/Camera.h"
#include "Sofia/Events/Event.h"

#include <entt.hpp>

namespace Sofia {

	class Entity;
	class Scene : public RefCounted
	{
		friend class Entity;
	public:
		Scene(const std::string& name = "Scene") noexcept : m_Name(name) {}
		~Scene();

		Entity CreateEntity(const std::string& name = {});
		void DestroyEntity(Entity entity);

		Entity SetCameraEntity(); //creates new camera entity
		Entity SetCameraEntity(Entity cameraEntity);
		Entity SetCameraEntity(Ref<Camera> camera); //creates new camera entity

		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height) noexcept;
	private:
		template<typename T>
		void OnComponentAdd(Entity entity, T& component);
	private:
		std::string m_Name;

		entt::registry m_Registry;
		entt::entity m_Camera = entt::null;

		glm::ivec2 m_ViewportSize = { 0, 0 };
	};
}
