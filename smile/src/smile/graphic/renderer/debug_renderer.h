/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"
#include "smile/common/primitive/collection/vector.h"

#include "view.h"
#include "resource/graphics_pipeline.h"
#include "resource/vertex_buffer.h"
#include "resource/frame_buffer.h"
#include "shader/constant_buffer.h"
#include "shader/binding_set.h"

namespace smile::graphic
{
    class DebugRenderer final : public foundation::MeyersSingleton< DebugRenderer >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const View &view );
        void OnRender( Framebuffer::Ref framebuffer );
        void EndScene();

        void SubmitLine( const DirectX::XMFLOAT3 &start, const DirectX::XMFLOAT3 &end, const DirectX::XMFLOAT4 &color );
        void SubmitLine( const DirectX::XMFLOAT3 &start,
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
    };
}