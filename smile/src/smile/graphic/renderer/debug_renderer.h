/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/foundation/meyers_singleton.h"

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/rhi/shader/shader.h"
#include "smile/graphic/rhi/render_state.h"

#include "smile/graphic/renderer/camera.h"

namespace smile::graphic
{
    class DebugRenderer final : public foundation::MeyersSingleton< DebugRenderer >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void OnRender();
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

        std::vector< VertexPosCol > m_LineList{};
        VertexBuffer::Ref m_pVertexBuffer;
        VertexShader::Ref m_pVertexShader;
        PixelShader::Ref m_pPixelShaderShader;

        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        Uint32 m_VertexCount{ 100 };
        const rhi::BufferLayout m_VertexLayout{ { rhi::Format::RGB32_FLOAT, "POSITION" },
            { rhi::Format::RGBA32_FLOAT, "COLOR" } };

        rhi::RenderState m_State{};
    };
}