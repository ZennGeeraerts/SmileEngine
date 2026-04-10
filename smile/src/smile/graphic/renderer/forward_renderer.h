/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"

#include "render_collector.h"
#include "view.h"
#include "graphics_state.h"
#include "material/material_instance.h"
#include "resource/graphics_pipeline.h"
#include "resource/frame_buffer.h"
#include "shader/constant_buffer.h"
#include "shader/binding_set.h"

namespace smile::graphic
{
    class ForwardRenderer final : public foundation::MeyersSingleton< ForwardRenderer >
    {
      public:
        struct PipelineKey final
        {
            PipelineKey( MaterialInstance::ConstRef materialInstance, const rhi::RenderState &renderState )
                : MaterialInstance{ materialInstance }, RenderState{ renderState }
            {
            }

            bool operator==( const PipelineKey &other ) const
            {
                return MaterialInstance->GetID() == other.MaterialInstance->GetID() && RenderState == other.RenderState;
            }

            bool operator!=( const PipelineKey &other ) const
            {
                return !( *this == other );
            }

            foundation::HashCode GetHashCode() const
            {
                foundation::HashCode hash = MaterialInstance->GetID().Hash();
                hash = foundation::HashCombine( hash, std::hash< rhi::RenderState >{}( RenderState ) );
                return hash;
            }

            MaterialInstance::ConstRef MaterialInstance;
            rhi::RenderState RenderState;
        };

        void Initialize();
        void ShutDown();

        void BeginScene( const View &view );
        void EndScene();
        void OnRender( Framebuffer::Ref framebuffer );

        void Submit( const DrawItem &drawItem );
        void Submit( DrawItem &&drawItem );

      private:
        void SetupMaterial( MaterialInstance::Ref materialInstance,
            const rhi::RenderState &renderState,
            GraphicsState &graphicsState );

        primitive::HashMap< PipelineKey, GraphicsPipeline::Ref >::Iterator
        CreatePipeline( MaterialInstance::ConstRef materialInstance, const rhi::RenderState &renderState );

        void ClearDrawList();

      private:
        RenderCollector m_RenderCollector;
        primitive::HashMap< PipelineKey, GraphicsPipeline::Ref > m_Pipelines;
        ConstantBuffer::Ref m_pCameraCB;
        ConstantBuffer::Ref m_PerObjectCB;
        BindingSet::Ref m_pBindingSet;

        ViewConstants m_ViewConstants{};
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::ForwardRenderer::PipelineKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ForwardRenderer::PipelineKey &key ) const
        {
            return key.GetHashCode();
        }
    };
}
