#pragma once

#include "Sofia/Scene/Entity.h"
#include "Sofia/Events/Event.h"

namespace Sofia {

	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;
	protected:
		template<typename T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}
		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
		}
		template<typename T>
		void RemoveComponent()
		{
			m_Entity.RemoveComponent<T>();
		}
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		Scene* GetScene() { return m_Entity.m_Scene; }
		glm::mat4& GetTransform() { return m_Entity.GetTransform(); }
		const glm::mat4& GetTransform() const { return m_Entity.GetTransform(); }

		TransformComponent& GetTransformComponent() { return m_Entity.GetTransformComponent(); }
		const TransformComponent& GetTransformComponent() const { return m_Entity.GetTransformComponent(); }

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnEvent(Event& e) {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnViewportResize(uint32_t width, uint32_t height) {}
	private:
		Entity m_Entity;
	};
}
