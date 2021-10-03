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

		// Deselect entities
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::End();

		// Inspector Panel
		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}

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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char tagBuffer[256];
			memset(tagBuffer, 0, sizeof(tagBuffer));
			strcpy_s(tagBuffer, sizeof(tagBuffer), tag.c_str());

			if (ImGui::InputText("Tag", tagBuffer, sizeof(tagBuffer)))
			{
				tag = std::string{ tagBuffer };
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transformComponent = entity.GetComponent<TransformComponent>();

				auto& position = transformComponent.Translation;
				ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.03f);

				auto& rotation = transformComponent.Rotation;
				ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation), 0.03f);

				auto& scale = transformComponent.Scale;
				ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.03f);

				ImGui::TreePop();
			}
		}
	}
}