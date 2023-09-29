#pragma once

#include "Sofia/Scene/Scene.h"

namespace Sofia {

	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene) noexcept : m_Scene(scene) {}

		void Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}
