/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"

#include "smile/core/math/color.h"

#include "graphics_state.h"
#include "resource/resource_manager.h"
#include "smile/graphic/rhi/command_list.h"
#include "smile/graphic/rhi/graphics_device.h"
#include "smile/graphic/rhi/swap_chain.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class RenderSystem final
    {
      public:
        RenderSystem();
        ~RenderSystem();

        void Initialize( const window::Window *pWindow );

        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height );

        void SetClearColor( const math::Color &color )
        {
            m_ClearColor = color;
        }

        void Clear();

        void BeginFrame();
        void EndFrame();

        void SetGraphicsState( const GraphicsState &state );
        void DrawIndexed( Uint32 indexCount );
        void Draw( Uint32 vertexCount );
        void Present();

        void FillVertexBuffer( VertexBuffer::Ref pVertexBuffer, void *pData, const Count vertexCount ) const;

        ResourceManager &GetResourceManager()
        {
            return m_ResourceManager;
        }

        rhi::RendererBackendType GetRendererAPI() const
        {
            return m_API;
        }

        rhi::GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice.get();
        }

        rhi::CommandList *GetImmediateCommandList() const
        {
            return m_pImmediateCommandList;
        }

      private:
        rhi::RendererBackendType m_API;
        Scope< rhi::GraphicsDevice > m_pDevice;
        rhi::CommandList *m_pImmediateCommandList;
        ResourceManager m_ResourceManager{};

        math::Color m_ClearColor{};

        memory::Ref< rhi::SwapChain > m_pSwapChain = nullptr;

        Index m_CurrentFrameIndex{ 0 };
        Index m_RenderedFrameIndex{ 0 };

        GraphicsState m_GraphicsState;
    };
}
