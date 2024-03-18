#include <pch.h>
#include "Scene.h"

#include "Sofia/Scene/Components.h"
#include "Sofia/Scene/Entity.h"
#include "Sofia/Renderer/Renderer2D.h"

namespace Sofia {

	Scene::~Scene()
	{
		m_Registry.clear();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entity = Entity(m_Registry.create(), this);

		if (!name.empty())
			entity.AddComponent<TagComponent>(name);
		else
			entity.AddComponent<TagComponent>("Entity");

		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));

		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_Handle);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Registry.view<TransformComponent>().each([=](entt::entity entity, TransformComponent& tc)
		{
			tc.Orientation = glm::mod(tc.Orientation, glm::two_pi<float>());
			glm::quat rotation = glm::quat(tc.Orientation);
			tc.Transform = glm::translate(glm::mat4(1.0f), tc.Position) * glm::scale(glm::toMat4(rotation), tc.Size);
		});

		{
			auto group = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::SubmitQuad(tc, sc.Color, sc.Texture, sc.TillingFactor);
			}
			Renderer2D::DrawQuads();
		}
	}
}
