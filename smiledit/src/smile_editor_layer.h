/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile.h"
#include "panels/world_hierarchy_panel.h"
#include "panels/content_browser_panel.h"

#include "smile/graphic/camera/editor_camera.h"

namespace smile
{
    class SmileEditorLayer final : public application::Layer
    {
      public:
        enum class GizmoType
        {
            None = 0,
            TranslateX = ( 1u << 0 ),
            TranslateY = ( 1u << 1 ),
            TranslateZ = ( 1u << 2 ),
            RotateX = ( 1u << 3 ),
            RotateY = ( 1u << 4 ),
            RotateZ = ( 1u << 5 ),
            RotateScreen = ( 1u << 6 ),
            ScaleX = ( 1u << 7 ),
            ScaleY = ( 1u << 8 ),
            ScaleZ = ( 1u << 9 ),
            Bounds = ( 1u << 10 ),
            ScaleXU = ( 1u << 11 ),
            ScaleYU = ( 1u << 12 ),
            ScaleZU = ( 1u << 13 ),

            Translate = TranslateX | TranslateY | TranslateZ,
            Rotate = RotateX | RotateY | RotateZ | RotateScreen,
            Scale = ScaleX | ScaleY | ScaleZ,
            ScaleU = ScaleXU | ScaleYU | ScaleZU, // universal
            Universal = Translate | Rotate | Scale
        };

        SmileEditorLayer();
        virtual ~SmileEditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate( primitive::Timestep deltaTime ) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent( window::Event &e ) override;

      private:
        bool OnKeyPressed( window::KeyPressedEvent &e );

        void NewProject();
        bool OpenProject();
        void OpenProject( const std::filesystem::path &path );
        void SaveProject();

        void SaveWorld();
        void SaveWorldAs();
        void OpenWorld( asset::AssetHandle handle );
        void NewWorld();

        void OnWorldPlay();
        void OnWorldSimulate();
        void OnWorldStop();

        void DuplicateEntity();

        // UI Panels
        void DrawToolbar();

      private:
        memory::Ref< world::World > m_pEditorWorld;
        graphic::EditorCamera m_EditorCamera;
        std::filesystem::path m_EditorWorldPath;

        DirectX::XMFLOAT2 m_ViewportSize = { 0.f, 0.f };
        bool m_IsViewportFocused = false;
        bool m_IsViewportHovered = false;

        GizmoType m_GizmoType = GizmoType::None;

        enum class WorldState
        {
            Edit = 0,
            Simulate = 1,
            Play = 2
        };
        WorldState m_WorldState = WorldState::Edit;

        // Panels
        world::WorldHierarchyPanel m_WorldHierarchyPanel;
        Scope< ContentBrowserPanel > m_pContentBrowserPanel;

        // Editor resources
        memory::Ref< graphic::Texture > m_pIconPlay = nullptr;
        memory::Ref< graphic::Texture > m_pIconSimulate = nullptr;
        memory::Ref< graphic::Texture > m_pIconStop = nullptr;
    };
}
