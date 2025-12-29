/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        instance_provider.cpp
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Abstract base class for providing an instance of a resource
 */
#include "smpch.h"
#include "instance_provider.h"

#include "smile/common/primitive/primitive.h"

#include <thread>

namespace smile::thread
{
    InstanceProviderHandle InstanceProvider::Get()
    {
#ifdef SM_C_DEBUG
        Uint32 tryCount{ 32 };
#endif

        while ( true )
        {
            Uint32 oldValue{ m_Used };
            const Index index{ primitive::CountTrailingOneBits( oldValue ) };

            if ( index >= 32 )
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for( 0s );

#ifdef SM_C_DEBUG
                if ( --tryCount < 0 )
                {
                    SM_ASSERT( false,
                        "Unable to give an instance after several tries, probably a recursion calling "
                        "InstanceProvider" );
                }
#endif
                continue;
            }

            const Uint32 value = oldValue | ( 1 << index );

            if ( m_Used.compare_exchange_weak( oldValue, value ) )
            {
                return InstanceProviderHandle{ this, index };
            }
        }
    }

    void InstanceProvider::Release( const InstanceProviderHandle &handle )
    {
        m_Used &= ~( 1u << handle.Idx );
    }
}