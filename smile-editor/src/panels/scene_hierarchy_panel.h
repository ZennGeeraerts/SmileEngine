#pragma once

#include "smile_engine/core/core.h"
#include "smile_engine/core/logger.h"
#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"

namespace Smile::Graphic
{
    class Material;
}

namespace Smile::Scene
{
    class SceneHierarchyPanel final
    {
      public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel( const Ref< Scene::Scene > &pScene );

        void SetContext( const Ref< Scene::Scene > &pScene );

        void OnImGuiRender();

        Scene::Entity GetSelectedEntity() const
        {
            return m_SelectedEntity;
        }

      private:
        void DrawEntityNode( Scene::Entity entity );

        static void DrawVector3Control( const std::string &label,
            DirectX::XMFLOAT3 &values,
            float resetValue = 0.0f,
            float columnWidth = 100.f );

        void DrawComponents( Scene::Entity entity );

        template < typename ComponentType, typename UIFunction >
        static void
        DrawComponent( const std::string &label, Scene::Entity entity, UIFunction uiFunction, bool bRemoveable = true );

        static void DrawMaterial( const Ref< Graphic::Material > &material );

      private:
        Ref< Scene::Scene > m_pContext;
        Scene::Entity m_SelectedEntity;
    };
}
