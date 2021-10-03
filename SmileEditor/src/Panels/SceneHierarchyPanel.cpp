#include "SceneHierarchyPanel.h"

#include <Libs/ImGui/imgui.h>
#include <Libs/ImGui/imgui_internal.h>

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

	void SceneHierarchyPanel::DrawVector3Control(const std::string& label, DirectX::XMFLOAT3& value, float resetValue, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		// X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });

		if (ImGui::Button("X", buttonSize))
			value.x = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.03f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		if (ImGui::Button("Y", buttonSize))
			value.y = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.03f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize))
			value.z = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.03f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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

				DrawVector3Control("Position", transformComponent.Translation);

				DirectX::XMFLOAT3 rotationDegrees = {};
				rotationDegrees.x = DirectX::XMConvertToDegrees(transformComponent.Rotation.x);
				rotationDegrees.y = DirectX::XMConvertToDegrees(transformComponent.Rotation.y);
				rotationDegrees.z = DirectX::XMConvertToDegrees(transformComponent.Rotation.z);
				DrawVector3Control("Rotation", rotationDegrees);
				transformComponent.Rotation.x = DirectX::XMConvertToRadians(rotationDegrees.x);
				transformComponent.Rotation.y = DirectX::XMConvertToRadians(rotationDegrees.y);
				transformComponent.Rotation.z = DirectX::XMConvertToRadians(rotationDegrees.z);

				DrawVector3Control("Scale", transformComponent.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary Camera", &cameraComponent.bPrimary);

				const uint32_t projectionTypeCount = 2;
				const char* projectionTypeStrs[projectionTypeCount]{ "Perspective", "Orthographic" };
				const char* currentProjectionTypeStr = projectionTypeStrs[static_cast<uint32_t>(cameraComponent.Camera.GetProjectionType())];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeStr))
				{
					for (uint32_t i{}; i < projectionTypeCount; ++i)
					{
						bool bSelected = currentProjectionTypeStr == projectionTypeStrs[i];
						if (ImGui::Selectable(projectionTypeStrs[i], bSelected))
						{
							currentProjectionTypeStr = projectionTypeStrs[i];
							cameraComponent.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (bSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				switch (cameraComponent.Camera.GetProjectionType())
				{
				case SceneCamera::ProjectionType::ePerspective:
				{
					float fov = cameraComponent.Camera.GetFOV();
					if (ImGui::DragFloat("FOV", &fov, 0.03f, 0.03f))
						cameraComponent.Camera.SetFOV(fov);

					float nearPlane = cameraComponent.Camera.GetPerspectiveNearPlane();
					if (ImGui::DragFloat("Near Plane", &nearPlane, 0.03f, 0.03f))
						cameraComponent.Camera.SetPerspectiveNearPlane(nearPlane);

					float farPlane = cameraComponent.Camera.GetPerspectiveFarPlane();
					if (ImGui::DragFloat("Far Plane", &farPlane, 0.03f, 0.03f))
						cameraComponent.Camera.SetPerspectiveFarPlane(farPlane);

					break;
				}
				case SceneCamera::ProjectionType::eOrthographic:
				{
					float size = cameraComponent.Camera.GetSize();
					if (ImGui::DragFloat("Size", &size, 0.03f, 0.03f))
						cameraComponent.Camera.SetSize(size);

					float nearPlane = cameraComponent.Camera.GetOrthographicNearPlane();
					if (ImGui::DragFloat("Near Plane", &nearPlane, 0.03f, 0.03f))
						cameraComponent.Camera.SetOrthographicNearPlane(nearPlane);

					float farPlane = cameraComponent.Camera.GetOrthographicFarPlane();
					if (ImGui::DragFloat("Far Plane", &farPlane, 0.03f, 0.03f))
						cameraComponent.Camera.SetOrthographicFarPlane(farPlane);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.bFixedAspectRatio);

					break;
				}
				}

				ImGui::TreePop();
			}
		}
	}
}