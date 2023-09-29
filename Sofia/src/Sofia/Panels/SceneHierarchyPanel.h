#pragma once

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Entity.h"

namespace Sofia {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene> scene) noexcept : m_Scene(scene) {}
		~SceneHierarchyPanel() = default;

		void SetScene(Ref<Scene> scene) noexcept;
		void SetSelected(Entity entity = {}) noexcept;
		Entity GetSelected() const noexcept { return m_Selected; }

		void OnUIRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_Selected;
	};
}
