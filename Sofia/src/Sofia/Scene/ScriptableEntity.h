#pragma once

#include "Sofia/Scene/Entity.h"
#include "Sofia/Events/Event.h"

namespace Sofia {

	class ScriptableEntity
	{
		friend class Scene;
	public:
		SOF_CORE virtual ~ScriptableEntity() = default;
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

		SOF_CORE Scene* GetScene() { return m_Entity.m_Scene; }
		SOF_CORE glm::mat4& GetTransform() { return m_Entity.GetTransform(); }
		SOF_CORE const glm::mat4& GetTransform() const { return m_Entity.GetTransform(); }

		SOF_CORE TransformComponent& GetTransformComponent() { return m_Entity.GetTransformComponent(); }
		SOF_CORE const TransformComponent& GetTransformComponent() const { return m_Entity.GetTransformComponent(); }

		SOF_CORE virtual void OnCreate() {}
		SOF_CORE virtual void OnDestroy() {}
		SOF_CORE virtual void OnEvent(Event& e) {}
		SOF_CORE virtual void OnUpdate(Timestep ts) {}
		SOF_CORE virtual void OnViewportResize(uint32_t width, uint32_t height) {}
	private:
		Entity m_Entity;
	};
}
