/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "ecs_types.h"

#include <vector>

namespace smile::ecs
{
    using ConstructorType = void ( * )( void * );
    using DestructorType = void ( * )( void * );

    template < typename ComponentType, typename... Args >
    inline void RawConstructObject( void *pObject, Args &&...params )
    {
        new ( pObject ) ComponentType( std::forward< Args >( params )... );
    }

    template < typename ComponentType, typename... Args >
    inline void ConstructObject( ComponentType *pObject, Args &&...params )
    {
        new ( pObject ) ComponentType{ std::forward< Args >( params )... };
    }

    template < typename ComponentType >
    inline void RawDestructObject( void *pObject )
    {
        static_cast< ComponentType * >( pObject )->ComponentType::~ComponentType();
    }

    class ComponentStorage
    {
      protected:
        ComponentStorage() = default;

        template < typename ComponentType >
        void Initialize( bool isOwnerIncluded )
        {
            m_pData = nullptr;
            m_Size = 0;
            m_Allocated = 0;
            m_pConstructor = RawConstructObject< ComponentType >;
            m_pDestructor = RawDestructObject< ComponentType >;
            m_ComponentSize = sizeof( ComponentType );
            m_IsOwnerData = isOwnerIncluded;
        }

      public:
        ~ComponentStorage();

        template < typename ComponentType, typename... Args >
        ComponentType &Append( IndexType index, Args &&...args )
        {
            Grow();
            ComponentType *pResult = reinterpret_cast< ComponentType * >( m_pData ) + m_Size - 1;
            ConstructObject< ComponentType >( pResult, std::forward< Args >( args )... );

            if ( m_pIndices )
                m_pIndices[m_Size - 1] = index;

            return *pResult;
        }

        void Swap( IndexType element1, IndexType element2 );
        IndexType RemoveSwap( IndexType deadEIndex );
        void PopSwap( IndexType a );
        void Clear();
        void Reset();

        template < typename ComponentType >
        ComponentType &Get( IndexType index )
        {
            return *reinterpret_cast< ComponentType * >( m_pData + m_ComponentSize * index );
        }

        template < typename ComponentType >
        const ComponentType &Get( IndexType index ) const
        {
            return *reinterpret_cast< const ComponentType * >( m_pData + m_ComponentSize * index );
        }

        void *GetRaw()
        {
            return m_pData;
        }
        const void *GetRaw() const
        {
            return m_pData;
        }
        void *GetRaw( IndexType index )
        {
            return m_pData + m_ComponentSize * index;
        }
        const void *GetRaw( IndexType index ) const
        {
            return m_pData + m_ComponentSize * index;
        }

        IndexType GetIndex( IndexType index ) const
        {
            return m_pIndices ? m_pIndices[index]
                              : reinterpret_cast< EntityHandleType * >( m_pData + m_ComponentSize * index )->GetIndex();
        }

        Uint32 GetSize() const
        {
            return m_Size;
        }
        Uint32 GetRawSize() const
        {
            return m_Size * m_ComponentSize;
        }

      private:
        void Grow();
        void Reallocate( Uint32 newSize );

      protected:
        Uint32 m_ComponentSize = 0;
        Uint32 m_Allocated = 0;
        Uint32 m_Size = 0;
        bool m_IsOwnerData = false;
        Byte *m_pData{ nullptr };
        IndexType *m_pIndices{ nullptr };

        ConstructorType m_pConstructor = nullptr;
        DestructorType m_pDestructor = nullptr;
    };

    template < typename ComponentType >
    class ComponentStorageHandler final : public ComponentStorage
    {
      public:
        ComponentStorageHandler( bool isOwnerStored )
        {
            Initialize< ComponentType >( isOwnerStored );
        }

        ComponentType *GetData()
        {
            return reinterpret_cast< ComponentType * >( m_pData );
        }
        const ComponentType *GetData() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData );
        }

        inline ComponentType &operator[]( int index )
        {
            return reinterpret_cast< ComponentType * >( m_pData )[index];
        }
        inline const ComponentType &operator[]( int index ) const
        {
            return reinterpret_cast< const ComponentType * >( m_pData )[index];
        }

        inline ComponentType *begin()
        {
            return reinterpret_cast< ComponentType * >( m_pData );
        }
        inline const ComponentType *begin() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData );
        }
        inline ComponentType *end()
        {
            return reinterpret_cast< ComponentType * >( m_pData + m_Size );
        }
        inline const ComponentType *end() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData + m_Size );
        }
    };

    template < typename ComponentType >
    ComponentStorageHandler< ComponentType > *ComponentStorageCast( ComponentStorage *pComponentStorage )
    {
        // TODO: error checking
        return reinterpret_cast< ComponentStorageHandler< ComponentType > * >( pComponentStorage );
    }
}