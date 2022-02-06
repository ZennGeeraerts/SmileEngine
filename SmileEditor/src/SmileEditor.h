#pragma once
#include <SmileEngine.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "SmileEngine/Renderer/EditorCamera.h"

namespace Smile
{
	class SmileEditorLayer final : public Layer
	{
	public:
		enum class GizmoType
		{
			eNone = 0,
			eTranslateX = (1u << 0),
			eTranslateY = (1u << 1),
			eTranslateZ = (1u << 2),
			eRotateX = (1u << 3),
			eRotateY = (1u << 4),
			eRotateZ = (1u << 5),
			eRotateScreen = (1u << 6),
			eScaleX = (1u << 7),
			eScaleY = (1u << 8),
			eScaleZ = (1u << 9),
			eBounds = (1u << 10),
			eScaleXU = (1u << 11),
			eScaleYU = (1u << 12),
			eScaleZU = (1u << 13),

			eTranslate = eTranslateX | eTranslateY | eTranslateZ,
			eRotate = eRotateX | eRotateY | eRotateZ | eRotateScreen,
			eScale = eScaleX | eScaleY | eScaleZ,
			eScaleU = eScaleXU | eScaleYU | eScaleZU, // universal
			eUniversal = eTranslate | eRotate | eScale
		};

		SmileEditorLayer();
		virtual ~SmileEditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void SaveSceneAs();
		void OpenScene();
		void OpenScene(const std::filesystem::path& filePath);
		void NewScene();

	private:
		Ref<Scene> m_pActiveScene;
		EditorCamera m_EditorCamera;

		Ref<Framebuffer> m_pFramebuffer;

		DirectX::XMFLOAT2 m_ViewportSize = { 0.f, 0.f };
		bool m_bViewportFocused = false;
		bool m_bViewportHovered = false;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		GizmoType m_GizmoType = GizmoType::eNone;
	};

	class SmileEditorGame final : public SmileGame
	{
	public:
		SmileEditorGame();
		virtual ~SmileEditorGame() = default;
	};
}

