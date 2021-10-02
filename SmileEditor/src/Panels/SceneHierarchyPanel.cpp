#include "SceneHierarchyPanel.h"

#include <Libs/ImGui/imgui.h>

#include "SmileEngine/Scene/Components.h"

namespace Smile
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& pScene)
	{
		SetContext(pScene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& pScene)
	{
		m_pContext = pScene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_pContext->m_Registry.each(
			[&] (auto entityID)
			{
				Entity entity{ entityID, m_pContext.get() };
				DrawEntityNode(entity);
			});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool bNodeExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (bNodeExpanded)
		{
			ImGui::TreePop();
		}
	}
}