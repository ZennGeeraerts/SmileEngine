/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "utils.h"

namespace smile::utils
{
    std::string WideStringToString( const std::wstring &wideString )
    {
        std::string str{ wideString.begin(), wideString.end() };
        return str;
    }

    std::wstring StringToWideString( const std::string &str )
    {
        std::wstring wideString{ str.begin(), str.end() };
        return wideString;
    }
}