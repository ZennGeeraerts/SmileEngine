/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/common/compiled/singleton.h"

#include "engine/graphic/renderer_api/resource/vertex_buffer.h"
#include "engine/graphic/renderer_api/shader/shader.h"

#include "engine/graphic/camera/camera.h"
#include "engine/graphic/camera/editor_camera.h"

namespace smile::graphic
{
    class DebugRenderer final : public compiled::Singleton< DebugRenderer >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void BeginScene( const EditorCamera &editorCamera );
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
        Ref< VertexBuffer > m_pVertexBuffer;
        Ref< Shader > m_pShader;

        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        Uint32 m_BufferSize{ 100 };
    };
}