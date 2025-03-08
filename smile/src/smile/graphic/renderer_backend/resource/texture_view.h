/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "foundation/compiled.h"
#include "memory/object.h"

namespace smile::graphic
{
    enum class TextureViewType : Uint32
    {
        Undefined,
        ShaderResource,
        RenderTarget,
        DepthStencil,
        ReadOnlyDepthStencil,
        UnorderedAccess,
        ShadingRate
    };

    enum class TextureViewAccess : Uint8
    {
        None = BIT( 0 ),
        Read = BIT( 1 ),
        Write = BIT( 2 ),
        ReadWrite = Read | Write
    };

    struct TextureViewDescriptor final
    {
        TextureViewType Type = TextureViewType::Undefined;
        Uint32 MipLevelCount = 0;
        TextureViewAccess Access = TextureViewAccess::None;
    };

    class TextureView : public memory::Object
    {
      public:
        TextureView( const TextureViewDescriptor &desc ) : m_Descriptor{ desc }
        {
        }

      private:
        TextureViewDescriptor m_Descriptor;
    };
}