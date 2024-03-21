#pragma once

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Components.h"

namespace Sofia {

	class Entity
	{
		friend class Scene;
		friend class ScriptableEntity;
	public:
		SOF_CORE Entity() = default;
		SOF_CORE Entity(entt::entity handle, Scene* scene) noexcept
			: m_Handle(handle), m_Scene(scene) {}
		SOF_CORE ~Entity() = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}
		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			SOF_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			auto& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdd<T>(*this, component);
			return component;
		}
		template<typename T>
		void RemoveComponent()
		{
			SOF_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Scene->m_Registry.erase<T>(m_Handle);
		}
		template<typename T>
		T& GetComponent()
		{
			SOF_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		SOF_CORE const Scene* GetScene() const noexcept { return m_Scene; }

		SOF_CORE glm::mat4& GetTransform() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
		SOF_CORE const glm::mat4& GetTransform() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

		SOF_CORE TransformComponent& GetTransformComponent() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
		SOF_CORE const TransformComponent& GetTransformComponent() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

		SOF_CORE operator uint32_t() const noexcept { return (uint32_t)m_Handle; }
		SOF_CORE operator bool() const noexcept { return m_Handle != entt::null && m_Scene; }
		SOF_CORE operator entt::entity() const noexcept { return m_Handle; }

		SOF_CORE bool operator==(const Entity& other) const noexcept { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }
		SOF_CORE bool operator!=(const Entity& other) const noexcept { return !(*this == other); }
	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
