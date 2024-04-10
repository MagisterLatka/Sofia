#pragma once

#include "Sofia/Scene/Scene.h"

namespace Sofia {

	class SceneSerializer
	{
	public:
		SOF_CORE SceneSerializer(Ref<Scene> scene) noexcept : m_Scene(scene) {}

		SOF_CORE void Serialize(const std::filesystem::path& filepath);
		SOF_CORE bool Deserialize(const std::filesystem::path& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}
