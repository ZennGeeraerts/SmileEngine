/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "foundation/pimpl.h"
#include "memory/ref.h"

// TODO: Remove dependency
#include "engine/graphic/renderer_api/graphics_device.h"
#include "engine/graphic/renderer_api/graphics_context.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class Shader;

    class RenderSystem final
    {
      public:
        RenderSystem();

        void Initialize( window::Window *pWindow );

        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height );

        void SetClearColor( const DirectX::XMFLOAT4 &color );
        void Clear();

        void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader );
        void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader );
        void Present();

        GraphicsDevice *GetGraphicsDevice() const;
        GraphicsContext *GetGraphicsContext() const;

      private:
        struct Opaque;
        foundation::PImpl< Opaque > m_pImpl;
    };
}
