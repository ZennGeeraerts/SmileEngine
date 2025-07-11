/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/array_view.h"

namespace smile::graphic
{
    Index AssignFrameData();
    Byte *AllocateFrameData( const Count size );
    void ReleaseFrameData( const Index frameIndex );

    template < typename ItemType >
    primitive::ArrayView< ItemType > AllocateFrameData( const Count itemCount )
    {
        return
        {
            reinterpret_cast< ItemType * >( AllocateFrameData( sizeof( ItemType ) * itemCount ), itemCount );
        }
    }
}