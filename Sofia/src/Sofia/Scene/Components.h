#pragma once

#include "Sofia/UUID.h"
#include "Sofia/Renderer/Texture.h"
#include "Sofia/Renderer/Camera.h"

namespace Sofia {

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const UUID& id) noexcept : ID(id) {}
		IDComponent(const IDComponent&) = default;
		IDComponent(IDComponent&&) = default;
		IDComponent& operator=(const IDComponent&) = default;
		IDComponent& operator=(IDComponent&&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) noexcept : Tag(tag) {}
		TagComponent(const TagComponent& other) = default;
		TagComponent(TagComponent&& other) = default;
		TagComponent& operator=(const TagComponent& other) = default;
		TagComponent& operator=(TagComponent&& other) = default;

		operator std::string& () noexcept { return Tag; }
		operator const std::string& () const noexcept { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Orientation = glm::vec3(0.0f); //In radians
		glm::vec3 Size = glm::vec3(1.0f);
		glm::mat4 Transform = glm::mat4(1.0f); //Edited by Scene::Update()

		TransformComponent() = default;
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
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(TransformComponent&& other) = default;
		TransformComponent& operator=(const TransformComponent& other) = default;
		TransformComponent& operator=(TransformComponent&& other) = default;

		operator glm::mat4& () noexcept { return Transform; }
		operator const glm::mat4& () const noexcept { return Transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;

		SpriteComponent() = default;
		SpriteComponent(glm::vec4 color, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f) noexcept
			: Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SpriteComponent(const SpriteComponent& other) = default;
		SpriteComponent(SpriteComponent&& other) = default;
		SpriteComponent& operator=(const SpriteComponent& other) = default;
		SpriteComponent& operator=(SpriteComponent&& other) = default;
	};

	struct CircleComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(glm::vec4 color, float thickness = 1.0f, float fade = 0.005f) noexcept
			: Color(color), Thickness(Thickness), Fade(fade) {}
		CircleComponent(const CircleComponent&) = default;
		CircleComponent(CircleComponent&&) = default;
		CircleComponent& operator=(const CircleComponent&) = default;
		CircleComponent& operator=(CircleComponent&&) = default;
	};

	struct CameraComponent
	{
		Ref<Sofia::Camera> Camera;

		CameraComponent() = default;
		CameraComponent(Ref<Sofia::Camera> camera) noexcept : Camera(camera) {}
		CameraComponent(const CameraComponent& other) = default;
		CameraComponent(CameraComponent&& other) = default;
		CameraComponent& operator=(const CameraComponent& other) = default;
		CameraComponent& operator=(CameraComponent&& other) = default;
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must be derrived from ScriptableEntity");
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
		template<typename T>
		T* GetInstance()
		{
			return dynamic_cast<T*>(Instance);
		}
	};
}
