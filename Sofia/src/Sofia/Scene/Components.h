#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	struct TagComponent
	{
		std::string Tag;

		SOF_CORE TagComponent() {}
		SOF_CORE TagComponent(const std::string& tag) : Tag(tag) {}
		SOF_CORE TagComponent(const TagComponent& other) : Tag(other.Tag) {}
		SOF_CORE TagComponent(TagComponent&& other) : Tag(other.Tag) {}
		SOF_CORE TagComponent& operator=(const TagComponent& other) { Tag = other.Tag; return *this; }
		SOF_CORE TagComponent& operator=(TagComponent&& other) { Tag = std::move(other.Tag); return *this; }

		SOF_CORE operator std::string& () { return Tag; }
		SOF_CORE operator const std::string& () const noexcept { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Orientation = glm::vec3(0.0f); //In radians
		glm::vec3 Size = glm::vec3(1.0f);
		glm::mat4 Transform = glm::mat4(1.0f); //Edited by Scene::Update()

		SOF_CORE TransformComponent() noexcept {}
		SOF_CORE TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{
			glm::quat orientation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(Transform, Size, orientation, Position, skew, perspective);
			Orientation = glm::eulerAngles(orientation);
		}
		SOF_CORE TransformComponent(glm::vec3 position, glm::vec3 orientation = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f))
			: Position(position), Orientation(orientation), Size(size) {}
		SOF_CORE TransformComponent(const TransformComponent& other)
			: Transform(other.Transform), Position(other.Position), Orientation(other.Orientation), Size(other.Size) {}
		SOF_CORE TransformComponent(TransformComponent&& other)
		{
			Transform = std::move(other.Transform);
			Position = std::move(other.Position);
			Orientation = std::move(other.Orientation);
			Size = std::move(other.Size);
		}

		SOF_CORE operator glm::mat4& () noexcept { return Transform; }
		SOF_CORE operator const glm::mat4& () const noexcept { return Transform; }
		SOF_CORE TransformComponent& operator=(const TransformComponent& other)
		{
			Transform = other.Transform;
			Position = other.Position;
			Orientation = other.Orientation;
			Size = other.Size;
			return *this;
		}
		SOF_CORE TransformComponent& operator=(TransformComponent&& other)
		{
			Transform = std::move(other.Transform);
			Position = std::move(other.Position);
			Orientation = std::move(other.Orientation);
			Size = std::move(other.Size);
			return *this;
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;

		SOF_CORE SpriteComponent() noexcept {}
		SOF_CORE SpriteComponent(glm::vec4 color, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f)
			: Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SOF_CORE SpriteComponent(const SpriteComponent& other)
			: Color(other.Color), Texture(other.Texture), TillingFactor(other.TillingFactor) {}
		SOF_CORE SpriteComponent(SpriteComponent&& other)
		{
			Color = std::move(other.Color);
			Texture = std::move(other.Texture);
			TillingFactor = std::move(other.TillingFactor);
		}

		SOF_CORE SpriteComponent& operator=(const SpriteComponent& other)
		{
			Color = other.Color;
			Texture = other.Texture;
			TillingFactor = other.TillingFactor;
			return *this;
		}
		SOF_CORE SpriteComponent& operator=(SpriteComponent&& other)
		{
			Color = std::move(other.Color);
			Texture = std::move(other.Texture);
			TillingFactor = std::move(other.TillingFactor);
			return *this;
		}
	};
}
