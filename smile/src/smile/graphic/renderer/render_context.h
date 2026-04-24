/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"

#include "smile/core/math/color.h"

#include "graphics_state.h"
#include "smile/graphic/rhi/command_list.h"
#include "smile/graphic/rhi/graphics_device.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class ResourceManager;
    class VertexBuffer;
    class ConstantBuffer;

    class RenderContext final
    {
      public:
        RenderContext();
        ~RenderContext();

        void Initialize( rhi::RendererBackendType api, ResourceManager *resourceManager );

        void Clear( const Framebuffer &framebuffer,
            const std::optional< math::Color > &color,
            std::optional< float > depth,
            std::optional< Uint8 > stencil );

        void Open();
        void Close();

        void SetGraphicsState( const GraphicsState &state );
        void DrawIndexed( Uint32 indexCount );
        void Draw( Uint32 vertexCount );
        void Present();

        void FillVertexBuffer( const VertexBuffer &vertexBuffer, void *pData, const Count vertexCount ) const;
        void FillConstantBuffer( const ConstantBuffer &constantBuffer ) const;

        rhi::GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice.get();
        }

        rhi::CommandList *GetImmediateCommandList() const
        {
            return m_pImmediateCommandList;
        }

      private:
        Scope< rhi::GraphicsDevice > m_pDevice;
        rhi::CommandList *m_pImmediateCommandList{ nullptr };

        ResourceManager *m_ResourceManager{ nullptr };
    };
}
