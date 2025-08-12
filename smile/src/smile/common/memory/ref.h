/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "counted.h"
#include "smile/common/foundation/hash_code.h"

#include <type_traits>

namespace smile::memory
{
    template < typename Type >
    class Ref final
    {
        static_assert( std::is_base_of_v< Counted, Type >, "Type does not derive from Counted" );

      public:
        inline Ref() : m_pInstance{ nullptr }
        {
        }

        inline Ref( const Ref &other ) : m_pInstance{ nullptr }
        {
            Reset( other.m_pInstance );
        }

        template < typename OtherType >
        inline Ref( const Ref< OtherType > &other ) : m_pInstance{ nullptr }
        {
            Reset( static_cast< Type * >( other.GetPointer() ) );
        }

        inline Ref( Ref &&other ) noexcept : m_pInstance{ other.m_pInstance }
        {
            other.m_pInstance = nullptr;
        }

        inline Ref( Type *pOther ) : m_pInstance{ nullptr }
        {
            Reset( pOther );
        }

        inline ~Ref()
        {
            Reset();
        }

        inline Ref &operator=( const Ref &other )
        {
            Reset( other.m_pInstance );
            return *this;
        }

        template < typename OtherType >
        inline Ref &operator=( const Ref< OtherType > &other )
        {
            Reset( static_cast< Type * >( other.GetPointer() ) );
            return *this;
        }

        inline Ref &operator=( Ref &&other ) noexcept
        {
            Reset();

            m_pInstance = other.m_pInstance;
            other.m_pInstance = nullptr;

            return *this;
        }

        inline Ref &operator=( Type *pOther )
        {
            Reset( pOther );
            return *this;
        }

        inline bool operator==( const Type *pOther ) const
        {
            return m_pInstance == pOther;
        }

        inline bool operator==( const Ref &other )
        {
            return m_pInstance == other.m_pInstance;
        }

        template < class OtherType >
        inline bool operator==( const Ref< OtherType > &other ) const
        {
            return m_pInstance == other.GetPointer();
        }

        inline bool operator!=( const Type *pOther ) const
        {
            return m_pInstance != pOther;
        }

        inline bool operator!=( const Ref &other ) const
        {
            return m_pInstance != other.m_pInstance;
        }

        template < class OtherType >
        inline bool operator!=( const Ref< OtherType > &other ) const
        {
            return m_pInstance != other.GetPointer();
        }

        inline bool operator<( const Type *pOther ) const
        {
            return m_pInstance < pOther;
        }

        inline bool operator<( const Ref &other ) const
        {
            return m_pInstance < other.m_pInstance;
        }

        template < typename OtherType >
        inline bool operator<( const Ref< OtherType > &other ) const
        {
            return m_pInstance < other.GetPointer();
        }

        inline Type *operator->() const
        {
            SM_ASSERT_MSG( m_pInstance, "Instance is nullptr" );
            return m_pInstance;
        }

        inline Type &operator*() const
        {
            SM_ASSERT_MSG( m_pInstance, "Instance is nullptr" );
            return *m_pInstance;
        }

        inline bool operator!() const
        {
            return !m_pInstance;
        }

        inline explicit operator bool() const
        {
            return m_pInstance != nullptr;
        }

        inline bool IsNull() const
        {
            return m_pInstance == nullptr;
        }

        inline Type *GetPointer() const
        {
            return m_pInstance;
        }

        template < typename OtherType >
        inline typename std::conditional< std::is_const< Type >::value, const OtherType, OtherType >::type &
        GetObject() const
        {
            SM_ASSERT_MSG( m_pInstance, "Instance is nullptr" );

            return static_cast< typename std::conditional< std::is_const< Type >::value, const Type, Type >::type & >(
                *m_pInstance );
        }

        inline void Reset( Type *pObject = nullptr )
        {
            auto pTemp = pObject;

            if ( pObject )
            {
                pObject->IncreaseRefCount();
            }

            if ( m_pInstance )
            {
                m_pInstance->DecreaseRefCount();
            }

            m_pInstance = pTemp;
        }

      private:
        Type *m_pInstance;
    };

    template < typename Object, typename... Args >
    constexpr Ref< Object > CreateRef( Args &&...args )
    {
        return Ref< Object >{ new Object( std::forward< Args >( args )... ) };
    }
}

namespace std
{
    template < typename Type >
    struct hash< smile::memory::Ref< Type > >
    {
        smile::foundation::HashCode operator()( const smile::memory::Ref< Type > &pRef ) const noexcept
        {
            return hash< Type * >()( pRef.GetPointer() );
        }
    };
}