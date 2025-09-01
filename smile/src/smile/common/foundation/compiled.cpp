/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "compiled.h"

#include "compiler.h"

#if SM_C_DEBUG

namespace smile
{
    void BoolResult::ReportFailure()
    {
#    ifdef SM_COMPILER_MSVC
        auto stop = foundation::HandleAssert( m_Message, "", __FILE__, __LINE__, __FUNCSIG__ );
#    else
        auto stop = foundation::HandleAssert( m_Message, "", __FILE__, __LINE__, __PRETTY_FUNCTION__ );
#    endif

        if ( stop )
            SM_DEBUGBREAK();
    }
}

#endif