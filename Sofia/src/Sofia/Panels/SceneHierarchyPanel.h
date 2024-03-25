#pragma once

#include "Sofia/Scene/Scene.h"
#include "Sofia/Scene/Entity.h"

namespace Sofia {

	class SceneHierarchyPanel
	{
	public:
		SOF_CORE SceneHierarchyPanel() = default;
		SOF_CORE SceneHierarchyPanel(Ref<Scene> scene) noexcept
			: m_Scene(scene) {}
		SOF_CORE ~SceneHierarchyPanel() = default;

		SOF_CORE void SetScene(Ref<Scene> scene) noexcept;
		SOF_CORE void SetSelected(Entity entity = {}) noexcept;
		SOF_CORE Entity GetSelected() const noexcept { return m_Selected; }

		SOF_CORE void OnUIRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_Selected;
	};
}
