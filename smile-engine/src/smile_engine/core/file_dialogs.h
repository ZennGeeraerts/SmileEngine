/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <string>

namespace smile
{
    class FileDialogs final
    {
      public:
        static std::string OpenFile( const char *filter );
        static std::string SaveFile( const char *filter );
    };
}