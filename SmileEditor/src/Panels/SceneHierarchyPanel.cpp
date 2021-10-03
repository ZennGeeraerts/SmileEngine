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