/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/renderer_api.h"

namespace graphicslib
{
    class GLRendererBackend final : public RendererAPI
    {
      public:
        GLRendererBackend();

        void Initialize( const common::PlatformData &platformData ) override;
        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;
        void SetClearColor( float r, float g, float b, float a ) override;
        void Clear() override;

        void Draw( Uint32 vertexCount ) override;
        void DrawIndexed( Uint32 indexCount ) override;
        void DrawLines( Uint32 vertexCount ) override;

        void SetLineWidth( float width ) override;
    };
}