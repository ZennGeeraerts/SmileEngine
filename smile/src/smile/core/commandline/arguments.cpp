/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "arguments.h"

namespace smile::commandline
{
    Arguments::Arguments( primitive::ArrayView< const char * > args ) : m_Arguments{ args }
    {
    }

    bool Arguments::HasArgument() const
    {
        if ( !m_Arguments.IsValidIndex( m_CurrentIndex ) )
        {
            return false;
        }

        const Arg currentArg{ m_Arguments[m_CurrentIndex], m_Arguments[m_CurrentIndex + 1] };
        return currentArg.IsValid();
    }

    Arg Arguments::Pop()
    {
        SM_ASSERT( HasArgument() );

        const Arg arg{ m_Arguments[m_CurrentIndex], m_Arguments[m_CurrentIndex + 1] };
        m_CurrentIndex += 2;

        return arg;
    }

    bool Arguments::IsFinished() const
    {
        return m_CurrentIndex >= m_Arguments.GetItemCount();
    }
}