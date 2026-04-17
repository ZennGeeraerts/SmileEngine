/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"
#include "smile/graphic/renderer/render_collector.h"
#include "smile/graphic/renderer/graphics_state.h"
#include "smile/graphic/renderer/material/material_instance.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_set.h"

namespace smile::graphic
{
    class ForwardRenderPass final : public RenderPass
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

        void Initialize() override;
        void ShutDown() override;

        void BeginPass( const View &view ) override;
        void Execute( Framebuffer::Ref framebuffer ) override;
        void EndPass() override;

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
    struct hash< smile::graphic::ForwardRenderPass::PipelineKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ForwardRenderPass::PipelineKey &key ) const
        {
            return key.GetHashCode();
        }
    };
}
