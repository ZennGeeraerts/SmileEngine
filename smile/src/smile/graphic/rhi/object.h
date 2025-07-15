/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::graphic::rhi
{
    enum class ObjectType : Uint32
    {
        D3D11_Resource,
        D3D11_Buffer,
        D3D11_RenderTargetView,
        D3D11_DepthStencilView,
        D3D11_ShaderResourceView,
        D3D11_UnorderedAccessView
    };

    struct Object final
    {
        Object( Uint64 i ) : Integer{ i }
        {
        }

        Object( void *p ) : Pointer{ p }
        {
        }

        template < typename Type >
        operator Type *() const
        {
            return static_cast< Type * >( Pointer );
        }

        union
        {
            Uint64 Integer;
            void *Pointer;
        };
    };
}