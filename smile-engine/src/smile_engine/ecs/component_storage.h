#pragma once

#include "ecs_types.h"

#include <vector>

namespace smile::ecs
{
    using ConstructorType = void ( * )( void * );
    using DestructorType = void ( * )( void * );

    template < typename ComponentType, typename... Args >
    inline void rawConstructObject( void *object, Args &&...params )
    {
        new ( object ) ComponentType( std::forward< Args >( params )... );
    }

    template < typename ComponentType, typename... Args >
    inline void constructObject( ComponentType *object, Args &&...params )
    {
        new ( object ) ComponentType{ std::forward< Args >( params )... };
    }

    template < typename ComponentType >
    inline void rawDestructObject( void *object )
    {
        static_cast< ComponentType * >( object )->ComponentType::~ComponentType();
    }

    class ComponentStorage
    {
      protected:
        ComponentStorage() = default;

        template < typename ComponentType >
        void initialize( bool owner_included )
        {
            data = nullptr;
            size = 0;
            allocated = 0;
            constructor = rawConstructObject< ComponentType >;
            destructor = rawDestructObject< ComponentType >;
            componentSize = sizeof( ComponentType );
            ownerData = owner_included;
        }

      public:
        ~ComponentStorage();

        template < typename ComponentType, typename... Args >
        ComponentType &append( IndexType index, Args &&...args )
        {
            grow();
            ComponentType *result = reinterpret_cast< ComponentType * >( data ) + size - 1;
            constructObject< ComponentType >( result, std::forward< Args >( args )... );

            if ( indices )
                indices[size - 1] = index;

            return *result;
        }

        void swap( IndexType element1, IndexType element2 );
        int removeSwap( IndexType dead_eindex );
        void popSwap( IndexType a );
        void clear();
        void reset();

        template < typename ComponentType >
        ComponentType &get( IndexType index )
        {
            return *reinterpret_cast< ComponentType * >( data + componentSize * index );
        }

        template < typename ComponentType >
        const ComponentType &get( IndexType index ) const
        {
            return *reinterpret_cast< const ComponentType * >( data + componentSize * index );
        }

        void *getRaw()
        {
            return data;
        }
        const void *getRaw() const
        {
            return data;
        }
        void *getRaw( IndexType index )
        {
            return data + componentSize * index;
        }
        const void *getRaw( IndexType index ) const
        {
            return data + componentSize * index;
        }

        IndexType getIndex( IndexType index ) const
        {
            return indices ? indices[index]
                           : reinterpret_cast< EntityHandleType * >( data + componentSize * index )->index;
        }

        Uint32 getSize() const
        {
            return size;
        }
        Uint32 getRawSize() const
        {
            return size * componentSize;
        }

      private:
        void grow();
        void reallocate( Uint32 new_size );

      protected:
        Uint32 componentSize = 0;
        Uint32 allocated = 0;
        Uint32 size = 0;
        bool ownerData = false;
        Byte *data{ nullptr };
        IndexType *indices{ nullptr };

        ConstructorType constructor = nullptr;
        DestructorType destructor = nullptr;
    };

    template < typename ComponentType >
    class ComponentStorageHandler final : public ComponentStorage
    {
      public:
        ComponentStorageHandler( bool owner_stored )
        {
            initialize< ComponentType >( owner_stored );
        }

        ComponentType *getData()
        {
            return reinterpret_cast< ComponentType * >( data );
        }
        const ComponentType *getData() const
        {
            return reinterpret_cast< const ComponentType * >( data );
        }

        inline ComponentType &operator[]( int index )
        {
            return reinterpret_cast< ComponentType * >( data )[index];
        }
        inline const ComponentType &operator[]( int index ) const
        {
            return reinterpret_cast< const ComponentType * >( data )[index];
        }

        inline ComponentType *begin()
        {
            return reinterpret_cast< ComponentType * >( data );
        }
        inline const ComponentType *begin() const
        {
            return reinterpret_cast< const ComponentType * >( data );
        }
        inline ComponentType *end()
        {
            return reinterpret_cast< ComponentType * >( data + size );
        }
        inline const ComponentType *end() const
        {
            return reinterpret_cast< const ComponentType * >( data + size );
        }
    };

    template < typename ComponentType >
    ComponentStorageHandler< ComponentType > *componentStorageCast( ComponentStorage *component_storage )
    {
        // TODO: error checking
        return reinterpret_cast< ComponentStorageHandler< ComponentType > * >( component_storage );
    }
}