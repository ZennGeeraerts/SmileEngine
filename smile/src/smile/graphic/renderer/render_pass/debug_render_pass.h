/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"

#include "render_pass.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_set.h"

namespace smile::graphic
{
    class RenderContext;
    class ShaderLibrary;

    class DebugRenderPass final : public RenderPass
    {
      public:
        DebugRenderPass( RenderContext &context, const ShaderLibrary &shaderLib ) noexcept;

        void Initialize() override;
        void ShutDown() override;

        void BeginPass( const View &view ) override;
        void Execute( Framebuffer::Ref framebuffer ) override;
        void EndPass() override;

        void DrawLine( const DirectX::XMFLOAT3 &start, const DirectX::XMFLOAT3 &end, const DirectX::XMFLOAT4 &color );
        void DrawLine( const DirectX::XMFLOAT3 &start,
            const DirectX::XMFLOAT3 &end,
            const DirectX::XMFLOAT4 &colorStart,
            const DirectX::XMFLOAT4 &colorEnd );

      private:
        void CreateFixedLineList();
        void CreateVertexBuffer();

      private:
        struct VertexPosCol final
        {
            DirectX::XMFLOAT3 Position;
            DirectX::XMFLOAT4 color;
        };

        primitive::Vector< VertexPosCol > m_LineList{};
        GraphicsPipeline::Ref m_pPipeline;
        VertexBuffer::Ref m_pVertexBuffer;
        ConstantBuffer::Ref m_pCameraCB;
        BindingSet::Ref m_pBindingSet;

        Count m_VertexCount{ 100 };

        RenderContext &m_Context;
        const ShaderLibrary &m_ShaderLib;
    };
}