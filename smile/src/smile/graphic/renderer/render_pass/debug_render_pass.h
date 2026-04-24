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
        DebugRenderPass( RenderContext &context,
            ResourceManager &resourceManager,
            const ShaderLibrary &shaderLib ) noexcept;

        void Initialize() override;
        void ShutDown() override;

        void BeginPass( const View &view ) override;
        void Execute( const Framebuffer &framebuffer ) override;
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
        GraphicsPipeline m_Pipeline;
        VertexBuffer m_VertexBuffer;
        const rhi::BufferLayout m_VertexLayout;
        ConstantBuffer m_CameraCB;
        BindingSet m_BindingSet;

        Count m_VertexCount{ 100 };

        RenderContext &m_Context;
        ResourceManager &m_ResourceManager;
        const ShaderLibrary &m_ShaderLib;
    };
}