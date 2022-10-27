#pragma once
#include <smile_engine.h>
#include "panels/scene_hierarchy_panel.h"
#include "panels/content_browser_panel.h"

#include "smile_engine/graphic/camera/editor_camera.h"

namespace smile
{
    class SmileEditorLayer final : public Layer
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

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onUpdate( Timestep delta_time ) override;
        virtual void onImGuiRender() override;
        virtual void onEvent( Event &e ) override;

      private:
        bool onKeyPressed( KeyPressedEvent &e );

        void saveScene();
        void saveSceneAs();
        void serializeScene( const Ref< scene::Scene > &scene, const std::filesystem::path &file_path );
        void openScene();
        void openScene( const std::filesystem::path &file_path );
        void newScene();

        void onScenePlay();
        void onSceneSimulate();
        void onSceneStop();

        void duplicateEntity();

        // UI Panels
        void drawToolbar();

      private:
        Ref< scene::Scene > activeScene;
        Ref< scene::Scene > editorScene;
        graphic::EditorCamera editorCamera;
        std::filesystem::path editorScenePath;

        DirectX::XMFLOAT2 viewportSize = { 0.f, 0.f };
        bool viewportFocused = false;
        bool viewportHovered = false;

        GizmoType gizmoType = GizmoType::None;

        enum class SceneState
        {
            Edit = 0,
            Simulate = 1,
            Play = 2
        };
        SceneState sceneState = SceneState::Edit;

        // Panels
        scene::SceneHierarchyPanel sceneHierarchyPanel;
        ContentBrowserPanel contentBrowserPanel;

        // Editor resources
        Ref< graphic::Texture2D > iconPlay = nullptr;
        Ref< graphic::Texture2D > iconSimulate = nullptr;
        Ref< graphic::Texture2D > iconStop = nullptr;
    };
}
