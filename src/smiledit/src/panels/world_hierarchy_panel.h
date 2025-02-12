/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "logging/logging.h"
#include "world/world.h"
#include "world/entity.h"

namespace smile::graphic
{
    class Material;
}

namespace smile::world
{
    class WorldHierarchyPanel final
    {
      public:
        WorldHierarchyPanel() = default;
        WorldHierarchyPanel( Ref< World > pWorld );

        void SetContext( Ref< World > pWorld );

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
        Ref< World > m_pContext;
        Entity m_SelectedEntity;
    };
}
