#pragma once

#include "smile_engine/core/core.h"
#include "smile_engine/core/logger.h"
#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"

namespace smile
{
    class Material;

    class SceneHierarchyPanel final
    {
      public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel( const Ref< Scene > &pScene );

        void SetContext( const Ref< Scene > &pScene );

        void OnImGuiRender();

        Entity GetSelectedEntity() const
        {
            return m_SelectedEntity;
        }

      private:
        void DrawEntityNode( Entity entity );

        static void DrawVector3Control( const std::string &label,
            DirectX::XMFLOAT3 &values,
            float resetValue = 0.0f,
            float columnWidth = 100.f );

        void DrawComponents( Entity entity );

        template < typename ComponentType, typename UIFunction >
        static void
        DrawComponent( const std::string &label, Entity entity, UIFunction uiFunction, bool bRemoveable = true );

        static void DrawMaterial( const Ref< Material > &pMaterial );

      private:
        Ref< Scene > m_pContext;
        Entity m_SelectedEntity;
    };
}
