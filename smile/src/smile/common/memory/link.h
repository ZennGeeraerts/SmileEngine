#pragma once

#include "counted.h"
#include "smile/common/foundation/derived_cast.h"

#include <type_traits>

namespace smile::memory
{
    template < typename Type >
    class Ref;

    template < typename Type >
    requires std::is_base_of_v< Counted, Type >
    class Link final
    {
      public:
        Link() noexcept : m_pInstance{ nullptr }
        {
        }

        Link( const Link &other ) : m_pInstance{ nullptr }
        {
            Reset( other.m_pInstance );
        }

        Link( Link &&other ) noexcept : m_pInstance{ other.m_pInstance }
        {
            other.m_pInstance = nullptr;
        }

        Link( Type *instance ) : m_pInstance{ nullptr }
        {
            Reset( instance );
        }

        ~Link()
        {
            Reset();
        }

        Link &operator=( const Link &other )
        {
            if ( this != &other )
            {
                Reset( other.m_pInstance );
            }

            return *this;
        }

        inline Link &operator=( const Ref< Type > &other );

        Link &operator=( Link &&other ) noexcept
        {
            Reset();

            m_pInstance = other.m_pInstance;
            other.m_pInstance = nullptr;

            return *this;
        }

        Link &operator=( Type *instance )
        {
            Reset( instance );

            return *this;
        }

        bool operator==( const Link< Type > &other ) const
        {
            return m_pInstance == other.m_pInstance;
        }

        bool operator==( const Type *instance ) const
        {
            return m_pInstance == instance;
        }

        bool operator!=( const Link< Type > &other ) const
        {
            return m_pInstance != other.m_pInstance;
        }

        bool operator!=( const Type *instance ) const
        {
            return m_pInstance != instance;
        }

        Type *operator->() const
        {
            SM_ASSERT_MSG( m_pInstance && !m_pInstance->IsDestructed(), "Instance is nullptr or destructed" );

            return m_pInstance;
        }

        Type &operator*() const
        {
            SM_ASSERT_MSG( m_pInstance && !m_pInstance->IsDestructed(), "Instance is nullptr or destructed" );

            return *m_pInstance;
        }

        bool operator!() const
        {
            return !m_pInstance;
        }

        explicit operator bool() const
        {
            return m_pInstance != nullptr;
        }

        bool IsNull() const
        {
            return m_pInstance == nullptr;
        }

        Type *GetPointer() const
        {
            return m_pInstance;
        }

        template < typename OtherType >
        inline typename std::conditional< std::is_const_v< Type >, const OtherType, OtherType >::type &GetObject() const
        {
            SM_ASSERT_MSG( m_pInstance, "Instance is nullptr" );

            return foundation::DerivedCast<
                typename std::conditional< std::is_const_v< Type >, const OtherType, OtherType >::type & >(
                *m_pInstance );
        }

        void Reset( Type *instance = nullptr )
        {
#ifdef SM_C_DEBUG
            if ( m_pInstance )
            {
                m_pInstance->DecreaseLinkCount();
            }
#endif

            if ( instance )
            {
#ifdef SM_C_DEBUG
                instance->IncreaseLinkCount();
#endif
                m_pInstance = instance;
            }
            else
            {
                m_pInstance = nullptr;
            }
        }

      private:
        Type *m_pInstance;
    };
}

#include "ref.h"

namespace smile::memory
{
    template < typename Type >
    requires std::is_base_of_v< Counted, Type >
    inline Link< Type > &Link< Type >::operator=( const Ref< Type > &other )
    {
        Reset( other.GetPointer() );

        return *this;
    }
}