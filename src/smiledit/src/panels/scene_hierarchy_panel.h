/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/foundation/compiled.h"
#include "engine/common/logger/logger.h"
#include "engine/core/scene/scene.h"
#include "engine/core/scene/entity.h"

namespace smile::graphic
{
    class Material;
}

namespace smile::scene
{
    class SceneHierarchyPanel final
    {
      public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel( Ref< Scene > pScene );

        void SetContext( Ref< Scene > pScene );

        void OnImGuiRender();

        Entity GetSelectedEntity() const
        {
            return m_SelectedEntity;
        }

      private:
        void DrawEntityNode( Entity entity, std::vector< Entity > &entitiesToAddChild );

        static bool DrawVector3Control( const std::string &label,
            DirectX::XMFLOAT3 &values,
            float resetValue = 0.0f,
            float columnWidth = 100.f );

        void DrawComponents( Entity entity );

        template < typename ComponentType, typename UIFunction >
        static void
        DrawComponent( const std::string &label, Entity entity, UIFunction uiFunction, bool bRemoveable = true );

        static void DrawMaterial( const Ref< graphic::Material > &material );

      private:
        Ref< Scene > m_pContext;
        Entity m_SelectedEntity;
    };
}
