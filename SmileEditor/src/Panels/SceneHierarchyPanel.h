#pragma once

#include "SmileEngine/Core/Core.h"
#include "SmileEngine/Core/Logger.h"
#include "SmileEngine/Scene/Scene.h"
#include "SmileEngine/Scene/Entity.h"

namespace Smile
{
	class SceneHierarchyPanel final
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& pScene);

		void SetContext(const Ref<Scene>& pScene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_pContext;
		Entity m_SelectedEntity;
	};
}

