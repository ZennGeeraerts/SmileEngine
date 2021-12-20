#include "SmileEditor.h"

#include "SmileEngine/Scene/SceneSerializer.h"
#include "SmileEngine/Utils/PlatformUtils.h"

#include <Libs/ImGui/imgui.h>
#include <Libs/ImGuizmo/ImGuizmo.h>

namespace Smile
{
	/*-----------------------------------------------------------------------------------------------------------*/
	/*---------------------------------------------- Editor Layer -----------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------------*/

	extern const std::filesystem::path g_ResourcePath;

	SmileEditorLayer::SmileEditorLayer()
		: Layer("SmileEditorLayer")
	{

	}

	void SmileEditorLayer::OnAttach()
	{
		Smile::RenderCommand::SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });

		m_pActiveScene = CreateRef<Scene>();
#if 0
		// Camera
		m_CameraEntity = m_pActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		// Gun
		m_GunEntity = m_pActiveScene->CreateEntity("Gun");
		auto& gunTransform = m_GunEntity.GetComponent<TransformComponent>();
		gunTransform.Translation.z += 15.f;
		gunTransform.Translation.y -= 2.f;
		gunTransform.Translation.x -= 2.f;
		gunTransform.Rotation.y = 90;
		gunTransform.Scale.x *= 5.f;
		gunTransform.Scale.y *= 5.f;
		gunTransform.Scale.z *= 5.f;

		auto pAlbedoMap = Texture2D::Create("Resources/Textures/base_albedo.jpg");
		auto pNormalMap = Texture2D::Create("Resources/Textures/base_normal.jpg");
		auto pMetalnessMap = Texture2D::Create("Resources/Textures/base_metallic.jpg");
		auto pRoughnessMap = Texture2D::Create("Resources/Textures/base_roughness.jpg");
		auto pAOMap = Texture2D::Create("Resources/Textures/base_AO.jpg");
		auto pEnvironmentMap = Texture2D::Create("Resources/Textures/Sunol_Cubemap.dds");

		m_pMaterial = CreateRef<Material>();
		m_pMaterial->SetUseAlbedoMap(true);
		m_pMaterial->SetAlbedo(pAlbedoMap);
		m_pMaterial->SetUseMetalnessMap(true);
		m_pMaterial->SetMetalness(pMetalnessMap);
		m_pMaterial->SetUseRoughnessMap(true);
		m_pMaterial->SetRoughness(pRoughnessMap);
		m_pMaterial->SetUseNormalMap(true);
		m_pMaterial->SetNormalMap(pNormalMap);
		m_pMaterial->SetUseAOMap(true);
		m_pMaterial->SetAOMap(pAOMap);

		/*m_pMaterial->SetAlbedo(DirectX::XMFLOAT3{ 0.9f, 0.1f, 0.1f });
		m_pMaterial->SetMetalness(1);
		m_pMaterial->SetRoughness(0.5f);*/

		auto staticMesh = m_GunEntity.AddComponent<StaticMeshComponent>("Resources/Meshes/drakefire_pistol_low.obj", m_pMaterial);
#endif
		// Framebuffer
		FramebufferData framebufferData{};
		framebufferData.Width = 1280;
		framebufferData.Height = 720;
		framebufferData.Attachments = { { FramebufferTextureFormat::eRGBA8, true }, FramebufferTextureFormat::eDepth, { FramebufferTextureFormat::eRGBA8, true } };
		m_pFramebuffer = Framebuffer::Create(framebufferData);
		m_pFramebuffer->SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });

		m_SceneHierarchyPanel.SetContext(m_pActiveScene);
	}

	void SmileEditorLayer::OnDetach()
	{

	}

	void SmileEditorLayer::OnUpdate(Timestep deltaTime)
	{
		auto framebufferData = m_pFramebuffer->GetData();
		if ((!Utils::CompareFloats(m_ViewportSize.x, static_cast<float>(framebufferData.Width)) || !Utils::CompareFloats(m_ViewportSize.y, static_cast<float>(framebufferData.Height)))
			&& (m_ViewportSize.x > 0) && (m_ViewportSize.y > 0))
		{
			m_pFramebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_pActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}
#if 0
		if (m_bViewportFocused)
		{
			auto& cameraTransform = m_CameraEntity.GetComponent<Smile::TransformComponent>();

			if (Smile::Input::IsKeyPressed(SM_LEFT))
				cameraTransform.Rotation.y -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
			if (Smile::Input::IsKeyPressed(SM_RIGHT))
				cameraTransform.Rotation.y += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
			if (Smile::Input::IsKeyPressed(SM_UP))
				cameraTransform.Rotation.x -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
			if (Smile::Input::IsKeyPressed(SM_DOWN))
				cameraTransform.Rotation.x += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);

			const auto forward = cameraTransform.GetForward();
			const auto right = cameraTransform.GetRight();
			DirectX::XMFLOAT3 move{};

			if (Smile::Input::IsKeyPressed('A'))
				move.x -= 1;
			if (Smile::Input::IsKeyPressed('D'))
				move.x += 1;
			if (Smile::Input::IsKeyPressed('S'))
				move.z -= 1;
			if (Smile::Input::IsKeyPressed('W'))
				move.z += 1;
			if (Smile::Input::IsKeyPressed(SM_SPACE))
				move.y += 1;
			if (Smile::Input::IsKeyPressed(SM_LCONTROL))
				move.y -= 1;

			DirectX::XMFLOAT3 dir{};
			dir.x = forward.x * move.z + right.x * move.x;
			//dir.y = forward.y * move.z + right.y * move.x;
			dir.z = forward.z * move.z + right.z * move.x;

			auto dirMat = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));
			DirectX::XMStoreFloat3(&dir, dirMat);

			cameraTransform.Translation.x += dir.x * m_CameraMoveSpeed * deltaTime;
			cameraTransform.Translation.y += dir.y * m_CameraMoveSpeed * deltaTime;
			cameraTransform.Translation.z += dir.z * m_CameraMoveSpeed * deltaTime;
		}

		{
			auto& gunTransform = m_GunEntity.GetComponent<TransformComponent>();
			gunTransform.Rotation.y += 1.f * deltaTime;
		}
#endif
		Smile::RenderCommand::Clear();
		m_pFramebuffer->Bind();
		m_pFramebuffer->Clear();
		m_pActiveScene->OnUpdate(deltaTime);
		m_pFramebuffer->Unbind();
	}

	void SmileEditorLayer::OnImGuiRender()
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		static bool bDockSpaceOpen = true;
		ImGui::Begin("DockSpace Demo", &bDockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float standardWindowMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = standardWindowMinSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"))
					Smile::SmileGame::GetInstance().ShutDown();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
		ImGui::Begin("Viewport");
		m_bViewportFocused = ImGui::IsWindowFocused();
		m_bViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		ImGui::Image(m_pFramebuffer->GetColor(0), ImVec2{ m_ViewportSize.x, m_ViewportSize.y });

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem");
			if (payload)
			{
				const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
				OpenScene(std::filesystem::path{ g_ResourcePath } / path);
			}

			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		m_GizmoType = GizmoType::eRotate;
		if (selectedEntity && (m_GizmoType != GizmoType::eNone))
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
			float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			auto cameraEntity = m_pActiveScene->GetPrimaryCameraEntity();
			if (cameraEntity)
			{
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;

				// Camera
				DirectX::XMFLOAT4X4 cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
				DirectX::XMMATRIX cameraTransformMat = DirectX::XMLoadFloat4x4(&cameraTransform);

				const DirectX::XMFLOAT4X4& cameraProjectionMatrix = camera.GetProjectionMatrix();
				DirectX::XMMATRIX cameraViewMatrixMat = DirectX::XMMatrixInverse(nullptr, cameraTransformMat);

				// Entity
				auto& entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
				auto entityTransform = entityTransformComponent.GetTransform();

				ImGuizmo::Manipulate(cameraViewMatrixMat.r->m128_f32, cameraProjectionMatrix.m[0], static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::MODE::LOCAL, entityTransform.m[0]);

				if (ImGuizmo::IsUsing())
				{
					DirectX::XMFLOAT3 translation{};
					DirectX::XMFLOAT3 rotation{};
					DirectX::XMFLOAT3 scale{};
					Math::DecomposeMatrix(entityTransform, translation, rotation, scale);

					entityTransformComponent.Translation = translation;

					//DirectX::XMVECTOR currentRotationVec = DirectX::XMLoadFloat3(&entityTransformComponent.Rotation);
					//DirectX::XMVECTOR deltaRotationVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&rotation), currentRotationVec);
					//DirectX::XMVECTOR newRotationVec = DirectX::XMVectorAdd(currentRotationVec, deltaRotationVec);
					//DirectX::XMStoreFloat3(&entityTransformComponent.Rotation, newRotationVec);
					entityTransformComponent.Rotation = rotation;
					entityTransformComponent.Scale = scale;
				}
			}
		}

		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::End();
	}

	void SmileEditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher{ e };
		dispatcher.Dispatch<KeyPressedEvent>(SM_BIND_EVENT_FN(SmileEditorLayer::OnKeyPressed));
	}

	bool SmileEditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 1)
			return false;

		bool bControlPressed = Input::IsKeyPressed(SM_LCONTROL) || Input::IsKeyPressed(SM_RCONTROL);
		bool bShiftPressed = Input::IsKeyPressed(SM_LSHIFT) || Input::IsKeyPressed(SM_RSHIFT);

		switch (e.GetKeyCode())
		{
		case 'S':
			if (bControlPressed && bShiftPressed)
				SaveSceneAs();
			break;
		case 'O':
			if (bControlPressed)
				OpenScene();
			break;
		case 'N':
			if (bControlPressed)
				NewScene();
			break;
		}

		return false;
	}

	void SmileEditorLayer::SaveSceneAs()
	{
		std::string filePath = Utils::SaveFile("Smile Scene (*.smile)\0*.smile\0");
		if (!filePath.empty())
		{
			SceneSerializer sceneSerializer{ m_pActiveScene };
			sceneSerializer.Serialize(filePath);
		}
		else
			SM_LOG_ERROR("SmileEditorLayer::SaveSceneAs > Failed to save scene. The file path was empty");
	}

	void SmileEditorLayer::OpenScene()
	{
		std::string filePath = Utils::OpenFile("Smile Scene (*.smile)\0*.smile\0");
		OpenScene(filePath);
	}

	void SmileEditorLayer::OpenScene(const std::filesystem::path& filePath)
	{
		if (!filePath.empty())
		{
			m_pActiveScene = CreateRef<Scene>();
			m_pActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_SceneHierarchyPanel.SetContext(m_pActiveScene);

			SceneSerializer sceneSerializer{ m_pActiveScene };
			sceneSerializer.Deserialize(filePath.string());
		}
	}

	void SmileEditorLayer::NewScene()
	{
		m_pActiveScene = CreateRef<Scene>();
		m_pActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_SceneHierarchyPanel.SetContext(m_pActiveScene);
	}

	/*-----------------------------------------------------------------------------------------------------------*/
	/*----------------------------------------------- Editor Game -----------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------------*/

	SmileGame* CreateGame()
	{
		// This application will get passed to the entry point of the engine
		// and will be deleted once the engine closes
		return new SmileEditorGame{};
	}

	SmileEditorGame::SmileEditorGame()
		: SmileGame("Smile Editor")
	{
		PushLayer(new SmileEditorLayer{});
	}
}