#pragma once

#include "smile_engine/core/core.h"
#include "smile_engine/core/logger.h"
#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"

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
        SceneHierarchyPanel( const Ref< scene::Scene > &scene );

        void setContext( const Ref< scene::Scene > &scene );

        void onImGuiRender();

        scene::Entity getSelectedEntity() const
        {
            return selectedEntity;
        }

      private:
        void drawEntityNode( scene::Entity entity );

        static void drawVector3Control( const std::string &label,
            DirectX::XMFLOAT3 &values,
            float reset_value = 0.0f,
            float column_width = 100.f );

        void drawComponents( scene::Entity entity );

        template < typename ComponentType, typename UIFunction >
        static void
        drawComponent( const std::string &label, scene::Entity entity, UIFunction ui_function, bool removeable = true );

        static void drawMaterial( const Ref< graphic::Material > &pMaterial );

      private:
        Ref< scene::Scene > context;
        scene::Entity selectedEntity;
    };
}
