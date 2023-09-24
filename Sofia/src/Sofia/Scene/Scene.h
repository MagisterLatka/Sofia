#pragma once

#include "Sofia/Core.h"
#include "Sofia/Timestep.h"

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

		void OnUpdate(Timestep ts);
	private:
		std::string m_Name;

		entt::registry m_Registry;
	};
}
