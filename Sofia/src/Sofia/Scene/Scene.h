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
		SOF_CORE Scene(const std::string& name = "Scene") noexcept
			: m_Name(name) {}
		SOF_CORE ~Scene();

		SOF_CORE Entity CreateEntity(const std::string& name = {});
		SOF_CORE void DestroyEntity(Entity entity);

		SOF_CORE void OnUpdate(Timestep ts);
	private:
		std::string m_Name;

		entt::registry m_Registry;
	};
}
