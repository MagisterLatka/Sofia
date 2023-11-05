#pragma once

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Components.h"

namespace Sofia {

	class Entity
	{
		friend class Scene;
		friend class ScriptableEntity;
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) noexcept
			: m_Handle(handle), m_Scene(scene) {}
		~Entity() = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Handle);
		}
		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			SOF_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdd<T>(*this, component);
			return component;
		}
		template<typename T, typename ...Args>
		T& AddReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdd<T>(*this, component);
			return component;
		}
		template<typename T>
		void RemoveComponent()
		{
			SOF_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}
		template<typename T>
		T& GetComponent()
		{
			SOF_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		const Scene* GetScene() const noexcept { return m_Scene; }

		UUID GetUUID() { return m_Scene->m_Registry.get<IDComponent>(m_Handle).ID; }

		glm::mat4& GetTransform() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
		const glm::mat4& GetTransform() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

		TransformComponent& GetTransformComponent() { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }
		const TransformComponent& GetTransformComponent() const { return m_Scene->m_Registry.get<TransformComponent>(m_Handle); }

		operator uint32_t() const noexcept { return (uint32_t)m_Handle; }
		operator bool() const noexcept { return m_Handle != entt::null && m_Scene; }
		operator entt::entity() const noexcept { return m_Handle; }

		bool operator==(const Entity& other) const noexcept { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const noexcept { return !(*this == other); }
	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
