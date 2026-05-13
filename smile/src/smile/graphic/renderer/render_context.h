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
    class VertexBuffer;
    class ConstantBuffer;

    class RenderContext final
    {
      public:
        RenderContext( rhi::CommandList &immediateCommandList ) noexcept;
        ~RenderContext();

        void Clear( const FramebufferAttachmentSet &attachmentSet,
            const std::optional< math::Color > &color,
            std::optional< float > depth,
            std::optional< Uint8 > stencil );

        void Open();
        void Close();

        void SetGraphicsState( const GraphicsState &state );
        void DrawIndexed( Uint32 indexCount );
        void Draw( Uint32 vertexCount );

        void FillVertexBuffer( const VertexBuffer &vertexBuffer, void *pData, const Count vertexCount ) const;
        void FillConstantBuffer( const ConstantBuffer &constantBuffer ) const;

        void CopyTexture( const Texture &dst,
            const rhi::TextureSlice &dstSlice,
            const Texture &src,
            const rhi::TextureSlice &srcSlice ) const;

        rhi::CommandList &GetImmediateCommandList() const
        {
            return m_ImmediateCommandList;
        }

      private:
        rhi::CommandList &m_ImmediateCommandList;
    };
}
