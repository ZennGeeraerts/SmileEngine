#include "smpch.h"
#include "component_storage.h"

namespace smile::ecs
{
    ComponentStorage::~ComponentStorage()
    {
        reset();
    }

    int ComponentStorage::removeSwap( Uint32 dead_eindex )
    {
        if ( dead_eindex >= size )
            return -1;

        int swap_handle = getIndex( size - 1 );

        if ( dead_eindex == size - 1 )
            swap_handle = -1;

        popSwap( dead_eindex );

        return swap_handle;
    }

    void ComponentStorage::popSwap( Uint32 a )
    {
        if ( size == 0 )
            return;

        --size;

        Byte *ia = data + componentSize * a;
        Byte *ie = data + componentSize * size;

        destructor( ia );

        if ( ia >= ie )
            return;

        memmove( ia, ie, componentSize );
        if ( indices )
            indices[a] = indices[size];
    }

    void ComponentStorage::grow()
    {
        Uint32 new_size = size + 1;

        if ( new_size > allocated )
            reallocate( allocated > 3 ? allocated * 2 : 8 );

        size = new_size;
    }

    void ComponentStorage::reallocate( Uint32 new_size )
    {
        data = reinterpret_cast< Byte * >( realloc( data, new_size * componentSize ) );

        if ( !ownerData )
            indices = reinterpret_cast< Uint32 * >( realloc( indices, new_size * sizeof( Uint32 ) ) );

        allocated = new_size;

        SM_ASSERT( data, "ComponentStorage::reallocate > data is nullptr" );
    }

    void ComponentStorage::clear()
    {
        Uint32 count = size;
        Byte *d = data;

        for ( ; count > 0; --count, d += componentSize )
            destructor( d );

        size = 0;
    }

    void ComponentStorage::reset()
    {
        clear();

        free( data );
        data = nullptr;

        free( indices );
        indices = nullptr;

        allocated = 0;
    }
}