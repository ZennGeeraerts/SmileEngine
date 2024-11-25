/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "component_storage.h"

namespace smile::ecs
{
    ComponentStorage::~ComponentStorage()
    {
        Reset();
    }

    void ComponentStorage::Swap( IndexType element1, IndexType element2 )
    {
        if ( element1 == element2 )
            return;

        Byte *pIA = m_pData + m_ComponentSize * element1;
        Byte *pIE = m_pData + m_ComponentSize * element2;

        std::swap_ranges( pIA, pIA + m_ComponentSize, pIE );

        if ( m_pIndices )
            std::swap( m_pIndices[element1], m_pIndices[element2] );
    }

    IndexType ComponentStorage::RemoveSwap( IndexType deadEIndex )
    {
        if ( deadEIndex >= m_Size )
            return -1;

        IndexType swapHandle = GetIndex( m_Size - 1 );

        if ( deadEIndex == m_Size - 1 )
            swapHandle = -1;

        PopSwap( deadEIndex );

        return swapHandle;
    }

    void ComponentStorage::PopSwap( IndexType a )
    {
        if ( m_Size == 0 )
            return;

        --m_Size;

        Byte *pIA = m_pData + m_ComponentSize * a;
        Byte *pIE = m_pData + m_ComponentSize * m_Size;

        m_pDestructor( pIA );

        if ( pIA >= pIE )
            return;

        memmove( pIA, pIE, m_ComponentSize );
        if ( m_pIndices )
            m_pIndices[a] = m_pIndices[m_Size];
    }

    void ComponentStorage::Grow()
    {
        Uint32 newSize = m_Size + 1;

        if ( newSize > m_Allocated )
            Reallocate( m_Allocated > 3 ? m_Allocated * 2 : 8 );

        m_Size = newSize;
    }

    void ComponentStorage::Reallocate( Uint32 newSize )
    {
        m_pData = reinterpret_cast< Byte * >( realloc( m_pData, newSize * m_ComponentSize ) );
        m_pIndices = reinterpret_cast< Uint32 * >( realloc( m_pIndices, newSize * sizeof( Uint32 ) ) );

        m_Allocated = newSize;

        SM_ASSERT( m_pData, "ComponentStorage::Reallocate > data is nullptr" );
    }

    void ComponentStorage::Clear()
    {
        Uint32 count = m_Size;
        Byte *pData = m_pData;

        for ( ; count > 0; --count, pData += m_ComponentSize )
            m_pDestructor( pData );

        m_Size = 0;
    }

    void ComponentStorage::Reset()
    {
        Clear();

        free( m_pData );
        m_pData = nullptr;

        free( m_pIndices );
        m_pIndices = nullptr;

        m_Allocated = 0;
    }
}