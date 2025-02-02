/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::logging
{
    struct LogSource final
    {
        LogSource() = default;
        LogSource( const char *fileName, int line, const char *functionName )
            : FileName{ fileName }, Line{ line }, FunctionName{ functionName }
        {
        }

        const char *FileName;
        int Line;
        const char *FunctionName;
    };
}