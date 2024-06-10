#pragma once

#include "Sofia/UUID.h"
#include "Sofia/Renderer/Texture.h"
#include "Sofia/Renderer/Camera.h"

namespace Sofia {

	struct IDComponent
	{
		UUID ID;

		SOF_CORE IDComponent() = default;
		SOF_CORE IDComponent(const UUID& id) noexcept : ID(id) {}
		SOF_CORE IDComponent(const IDComponent&) = default;
		SOF_CORE IDComponent(IDComponent&&) = default;
		SOF_CORE IDComponent& operator=(const IDComponent&) = default;
		SOF_CORE IDComponent& operator=(IDComponent&&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		SOF_CORE TagComponent() = default;
		SOF_CORE TagComponent(const std::string& tag) : Tag(tag) {}
		SOF_CORE TagComponent(const TagComponent& other) = default;
		SOF_CORE TagComponent(TagComponent&& other) = default;
		SOF_CORE TagComponent& operator=(const TagComponent& other) = default;
		SOF_CORE TagComponent& operator=(TagComponent&& other) = default;

		SOF_CORE operator std::string& () { return Tag; }
		SOF_CORE operator const std::string& () const noexcept { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Orientation = glm::vec3(0.0f); //In radians
		glm::vec3 Size = glm::vec3(1.0f);
		glm::mat4 Transform = glm::mat4(1.0f); //Edited by Scene::Update()

		SOF_CORE TransformComponent() = default;
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
		SOF_CORE TransformComponent(const TransformComponent& other) = default;
		SOF_CORE TransformComponent(TransformComponent&& other) = default;
		SOF_CORE TransformComponent& operator=(const TransformComponent& other) = default;
		SOF_CORE TransformComponent& operator=(TransformComponent&& other) = default;

		SOF_CORE operator glm::mat4& () noexcept { return Transform; }
		SOF_CORE operator const glm::mat4& () const noexcept { return Transform; }
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

	struct SpriteComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;

		SOF_CORE SpriteComponent() = default;
		SOF_CORE SpriteComponent(glm::vec4 color, const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f)
			: Color(color), Texture(texture), TillingFactor(tillingFactor) {}
		SOF_CORE SpriteComponent(const SpriteComponent& other) = default;
		SOF_CORE SpriteComponent(SpriteComponent&& other) = default;
		SOF_CORE SpriteComponent& operator=(const SpriteComponent& other) = default;
		SOF_CORE SpriteComponent& operator=(SpriteComponent&& other) = default;
	};

	struct CameraComponent
	{
		Ref<Sofia::Camera> Camera;

		SOF_CORE CameraComponent() = default;
		SOF_CORE CameraComponent(Ref<Sofia::Camera> camera) : Camera(camera) {}
		SOF_CORE CameraComponent(const CameraComponent& other) = default;
		SOF_CORE CameraComponent(CameraComponent&& other) = default;
		SOF_CORE CameraComponent& operator=(const CameraComponent& other) = default;
		SOF_CORE CameraComponent& operator=(CameraComponent&& other) = default;
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
