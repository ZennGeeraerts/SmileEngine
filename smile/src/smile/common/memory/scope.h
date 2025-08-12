/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::memory
{
    template < typename Type >
    class Scope final
    {
        template < typename OtherType >
        friend class Scope;

      public:
        constexpr Scope() = default;

        Scope( const Scope & ) = delete;

        constexpr Scope( Scope &&other ) : m_pObject{ other.m_pObject }
        {
            other.m_pObject = nullptr;
        }

        constexpr Scope( Type *pObject ) : m_pObject{ pObject }
        {
        }

        ~Scope()
        {
            delete m_pObject;
        }

        Scope &operator=( const Scope & ) = delete;

        template < typename OtherType >
        constexpr Scope &operator=( Scope< OtherType > &&other )
        {
            if ( m_pObject )
            {
                delete m_pObject;
            }

            m_pObject = other.m_pObject;
            other.m_pObject = nullptr;
            return *this;
        }

        constexpr explicit operator bool() const
        {
            return m_pObject != nullptr;
        }

        constexpr Type &operator*() const
        {
            SM_ASSERT( m_pObject );
            return *m_pObject;
        }

        constexpr Type *operator->() const
        {
            SM_ASSERT( m_pObject );
            return m_pObject;
        }

        constexpr void Reset( Type *pNewObject = nullptr )
        {
            if ( m_pObject )
            {
                delete m_pObject;
            }

            m_pObject = pNewObject;
        }

        constexpr Type *Release()
        {
            auto pResult = m_pObject;
            m_pObject = nullptr;

            return pResult;
        }

        constexpr Type *GetPointer()
        {
            return m_pObject;
        }

        constexpr const Type *GetPointer() const
        {
            return m_pObject;
        }

        bool operator==( const Scope &other ) const
        {
            return m_pObject == other.m_pObject;
        }

      private:
        Type *m_pObject{ nullptr };
    }

    template < typename Type, typename... Args >
    constexpr Scope< Type > CreateScope( Args &&...args )
    {
        return Scope< Type >{ new Scope( std::forward< Args >( args )... ) };
    }
}