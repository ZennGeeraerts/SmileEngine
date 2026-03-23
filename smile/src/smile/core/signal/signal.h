/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        signal.h
 * @author      Zenn Geeraerts
 * @created     13 March 2026
 * @brief       Send and receive messages
 */
#pragma once

#include "smile/common/primitive/collection/vector.h"

#include <functional>

namespace smile::signal
{
    template < typename Type >
    class Sink;

    template < typename Type >
    class SignalHandler;

    template < typename Return, typename... Args >
    class SignalHandler< Return( Args... ) >
    {
        friend class Sink< SignalHandler< Return( Args... ) > >;

        using Func = std::function< Return( Args... ) >;

      public:
        ~SignalHandler() = default;

        void Publish( Args... args ) const
        {
            for ( auto i{ m_Calls.GetItemCount() }; i > 0; --i )
            {
                m_Calls[i - 1]( args... );
            }
        }

        template < typename CollectFunc >
        void Collect( CollectFunc func, Args... args ) const
        {
            for ( auto i{ m_Calls.GetItemCount() }; i > 0; --i )
            {
                if constexpr ( std::is_void_v< Return > )
                {
                    m_Calls[i - 1]( args... );
                }
                else
                {
                    func( m_Calls[i - 1]( args... ) );
                }
            }
        }

        [[nodiscard]] Count GetItemCount() const noexcept
        {
            return m_Calls.GetItemCount();
        }

        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return m_Calls.IsEmpty();
        }

      private:
        primitive::Vector< Func > m_Calls;
    };

    class Connection final
    {
        template < typename >
        friend class Sink;

      public:
        Connection() = default;

        void Close()
        {
            if ( m_Disconnect )
            {
                m_Disconnect( m_Signal );
                m_Disconnect = nullptr;
            }
        }

      private:
        Connection( std::function< void( void * ) > fn, void *signal ) : m_Disconnect{ fn }, m_Signal{ signal }
        {
        }

        std::function< void( void * ) > m_Disconnect;
        void *m_Signal;
    };

    template < typename Return, typename... Args >
    class Sink< SignalHandler< Return( Args... ) > >
    {
      public:
        using SignalType = SignalHandler< Return( Args... ) >;
        using FuncType = SignalType::Func;

        Sink( SignalType &signal ) noexcept : m_Signal{ &signal }
        {
        }

        Connection Connect( FuncType call )
        {
            Disconnect( call );

            AssureSignal().m_Calls.PushBack( std::move( call ) );

            std::function< void( void * ) > conn = [call]( void *signal ) { Close( signal, call ); };

            return { conn, m_Signal };
        }

        void Disconnect( FuncType call )
        {
            DisconnectIf(
                [&call]( const auto &fn )
                {
                    return fn.target_type() == call.target_type() &&
                           fn.template target< FuncType >() == call.template target< FuncType >();
                } );
        }

        void Disconnect()
        {
            AssureSignal().m_Calls.Clear();
        }

        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return m_Signal->m_Calls.IsEmpty();
        }

      private:
        [[nodiscard]] SignalType &AssureSignal() const noexcept
        {
            SM_ASSERT( m_Signal );
            return *m_Signal;
        }

        template < typename Predicate >
        void DisconnectIf( Predicate callback )
        {
            auto &calls = AssureSignal().m_Calls;

            for ( auto i{ calls.GetItemCount() }; i > 0; --i )
            {
                auto &elem = calls[i - 1];
                if ( callback( elem ) )
                {
                    elem = std::move( calls.GetLastItem() );
                    calls.PopBack();
                }
            }
        }

        static void Close( void *signal, FuncType call )
        {
            Sink{ *static_cast< SignalType * >( signal ) }.Disconnect( call );
        }

      private:
        SignalType *m_Signal;
    };

    template < typename Return, typename... Args >
    Sink( SignalHandler< Return( Args... ) > & ) -> Sink< SignalHandler< Return( Args... ) > >;
}