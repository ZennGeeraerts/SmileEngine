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
		m_EditorCamera = EditorCamera{ 30.f, 1.778f, 0.1f, 2500.f };

		// Icon
		m_pIconPlay = Texture2D::Create("EditorResources/Icons/PlayButton.png");
		m_pIconStop = Texture2D::Create("EditorResources/Icons/StopButton.png");

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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_pActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}

		Smile::RenderCommand::Clear();
		m_pFramebuffer->Bind();
		m_pFramebuffer->Clear();

		switch (m_SceneState)
		{
		case SceneState::eEdit:
		{
			if (m_bViewportFocused)
				m_EditorCamera.OnUpdate(deltaTime);

			m_pActiveScene->OnUpdateEditor(deltaTime, m_EditorCamera);
			break;
		}
		case SceneState::ePlay:
		{
			m_pActiveScene->OnUpdateRuntime(deltaTime);
			break;
		}
		}

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
			const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("ContentBrowserItem");
			if (pPayload)
			{
				const wchar_t* path = static_cast<const wchar_t*>(pPayload->Data);
				OpenScene(std::filesystem::path{ g_ResourcePath } / path);
			}

			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && (m_GizmoType != GizmoType::eNone) && (m_SceneState == SceneState::eEdit))
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
			float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			//// Runtime camera
			//auto cameraEntity = m_pActiveScene->GetPrimaryCameraEntity();
			//if (cameraEntity)
			// {
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;

			//// Camera
			//DirectX::XMFLOAT4X4 cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
			//DirectX::XMMATRIX cameraTransformMat = DirectX::XMLoadFloat4x4(&cameraTransform);

			//const DirectX::XMFLOAT4X4& cameraProjectionMatrix = camera.GetProjectionMatrix();
			//DirectX::XMMATRIX cameraViewMatrixMat = DirectX::XMMatrixInverse(nullptr, cameraTransformMat);

			// Editor camera
			const DirectX::XMFLOAT4X4& cameraProjectionMatrix = m_EditorCamera.GetProjectionMatrix();
			const DirectX::XMFLOAT4X4& cameraViewMatrix = m_EditorCamera.GetViewMatrix();

			// Entity
			auto& entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
			auto entityTransform = entityTransformComponent.GetTransform();
			auto entityRotation = DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees(entityTransformComponent.Rotation.x), 
				DirectX::XMConvertToDegrees(entityTransformComponent.Rotation.y), DirectX::XMConvertToDegrees(entityTransformComponent.Rotation.z) };

			// Snapping
			bool bSnapping = Input::IsKeyPressed(SM_LCONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == GizmoType::eRotate)
				snapValue = 45.f;
			float snapValues[3]{ snapValue, snapValue, snapValue };

			ImGuizmo::RecomposeMatrixFromComponents(&entityTransformComponent.Translation.x, &entityRotation.x, &entityTransformComponent.Scale.x, *entityTransform.m);
			ImGuizmo::Manipulate(*cameraViewMatrix.m, *cameraProjectionMatrix.m, static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::MODE::LOCAL, *entityTransform.m, nullptr, bSnapping ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				ImGuizmo::DecomposeMatrixToComponents(*entityTransform.m, &entityTransformComponent.Translation.x, &entityRotation.x, &entityTransformComponent.Scale.x);
				entityTransformComponent.Rotation = { DirectX::XMConvertToRadians(entityRotation.x), DirectX::XMConvertToRadians(entityRotation.y), DirectX::XMConvertToRadians(entityRotation.z) };
			}
		}

		ImGui::End();

		ImGui::PopStyleVar();

		DrawToolbar();

		ImGui::End();
	}

	void SmileEditorLayer::DrawToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 });

		const auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHoveredColor = colors[ImGuiCol_ButtonHovered];
		const auto& buttonActiveColor = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ buttonHoveredColor.x, buttonHoveredColor.y, buttonHoveredColor.z, 0.5f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActiveColor.x, buttonActiveColor.y, buttonActiveColor.z, 0.5f });

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		
		Ref<Texture2D> pStateIcon = (m_SceneState == SceneState::eEdit) ? m_pIconPlay : m_pIconStop;
		float iconSize{ ImGui::GetWindowHeight() - 4.f };
		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (iconSize * 0.5f));
		if (ImGui::ImageButton(static_cast<ImTextureID>(pStateIcon->GetData()), ImVec2{ iconSize, iconSize }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 }, 0))
		{
			if (m_SceneState == SceneState::eEdit)
				OnScenePlay();
			else if (m_SceneState == SceneState::ePlay)
				OnSceneStop();
		}
		
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void SmileEditorLayer::OnEvent(Event& e)
	{
		if (m_bViewportHovered)
			m_EditorCamera.OnEvent(e);

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

		// Gizmos
		case 'Q':
			m_GizmoType = GizmoType::eNone;
			break;
		case 'W':
			m_GizmoType = GizmoType::eTranslate;
			break;
		case 'E':
			m_GizmoType = GizmoType::eRotate;
			break;
		case 'R':
			m_GizmoType = GizmoType::eScale;
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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_pActiveScene);

			SceneSerializer sceneSerializer{ m_pActiveScene };
			sceneSerializer.Deserialize(filePath.string());
		}
	}

	void SmileEditorLayer::NewScene()
	{
		m_pActiveScene = CreateRef<Scene>();
		m_pActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_pActiveScene);
	}

	void SmileEditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::ePlay;
	}

	void SmileEditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::eEdit;
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