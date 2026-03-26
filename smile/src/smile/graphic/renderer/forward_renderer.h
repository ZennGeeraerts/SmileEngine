/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"

#include "render_collector.h"
#include "camera.h"
#include "graphics_state.h"
#include "material/material.h"
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
            PipelineKey( Material::ConstRef material, const rhi::RenderState &renderState )
                : Material{ material }, RenderState{ renderState }
            {
            }

            bool operator==( const PipelineKey &other ) const
            {
                return Material->GetID() == other.Material->GetID() && RenderState == other.RenderState;
            }

            bool operator!=( const PipelineKey &other ) const
            {
                return !( *this == other );
            }

            foundation::HashCode GetHashCode() const
            {
                foundation::HashCode hash = Material->GetID().Hash();
                hash = foundation::HashCombine( hash, std::hash< rhi::RenderState >{}( RenderState ) );
                return hash;
            }

            Material::ConstRef Material;
            rhi::RenderState RenderState;
        };

        void Initialize();
        void ShutDown();

        void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void EndScene();
        void OnRender( Framebuffer::Ref framebuffer );

        void Submit( const DrawCommand &drawItem );
        void Submit( DrawCommand &&drawItem );

      private:
        void
        SetupMaterial( Material::ConstRef material, const rhi::RenderState &renderState, GraphicsState &graphicsState );

        primitive::HashMap< PipelineKey, GraphicsPipeline::Ref >::Iterator CreatePipeline( Material::ConstRef material,
            const rhi::RenderState &renderState );

        void ClearDrawList();

      private:
        RenderCollector m_RenderCollector;
        primitive::HashMap< PipelineKey, GraphicsPipeline::Ref > m_Pipelines;
        ConstantBuffer::Ref m_pCameraCB;
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
