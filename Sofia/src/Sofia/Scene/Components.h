#pragma once

#include "Sofia/Renderer/Texture.h"

namespace Sofia {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() {}
		TagComponent(const std::string& tag) noexcept : Tag(tag) {}
		TagComponent(const TagComponent& other) noexcept : Tag(other.Tag) {}
		TagComponent(TagComponent&& other) : Tag(other.Tag) {}
		TagComponent& operator=(const TagComponent& other) noexcept { Tag = other.Tag; return *this; }
		TagComponent& operator=(TagComponent&& other) { Tag = std::move(other.Tag); return *this; }

		operator std::string& () noexcept { return Tag; }
		operator const std::string& () const noexcept { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Orientation = glm::vec3(0.0f); //In radians
		glm::vec3 Size = glm::vec3(1.0f);
		glm::mat4 Transform = glm::mat4(1.0f); //Edited by Scene::Update()

		TransformComponent() noexcept {}
		TransformComponent(const glm::mat4& transform) noexcept
			: Transform(transform)
		{
			glm::quat orientation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(Transform, Size, orientation, Position, skew, perspective);
			Orientation = glm::eulerAngles(orientation);
		}
		TransformComponent(glm::vec3 position, glm::vec3 orientation = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f)) noexcept
			: Position(position), Orientation(orientation), Size(size) {}
		TransformComponent(const TransformComponent& other) noexcept
			: Transform(other.Transform), Position(other.Position), Orientation(other.Orientation), Size(other.Size) {}
		TransformComponent(TransformComponent&& other)
		{
			Transform = std::move(other.Transform);
			Position = std::move(other.Position);
			Orientation = std::move(other.Orientation);
			Size = std::move(other.Size);
		}

		operator glm::mat4& () noexcept { return Transform; }
		operator const glm::mat4& () const noexcept { return Transform; }
		TransformComponent& operator=(const TransformComponent& other) noexcept
		{
			Transform = other.Transform;
			Position = other.Position;
			Orientation = other.Orientation;
			Size = other.Size;
			return *this;
		}
		TransformComponent& operator=(TransformComponent&& other)
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

		SpriteComponent() noexcept {}
		SpriteComponent(glm::vec4 color, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f) noexcept
			: Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SpriteComponent(const SpriteComponent& other) noexcept
			: Color(other.Color), Texture(other.Texture), TillingFactor(other.TillingFactor) {}
		SpriteComponent(SpriteComponent&& other)
		{
			Color = std::move(other.Color);
			Texture = std::move(other.Texture);
			TillingFactor = std::move(other.TillingFactor);
		}

		SpriteComponent& operator=(const SpriteComponent& other) noexcept
		{
			Color = other.Color;
			Texture = other.Texture;
			TillingFactor = other.TillingFactor;
			return *this;
		}
		SpriteComponent& operator=(SpriteComponent&& other)
		{
			Color = std::move(other.Color);
			Texture = std::move(other.Texture);
			TillingFactor = std::move(other.TillingFactor);
			return *this;
		}
	};
}
