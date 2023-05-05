/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <string>

namespace smile::utils
{
    std::string WideStringToString( const std::wstring &wideString );
    std::wstring StringToWideString( const std::string &str );
}
