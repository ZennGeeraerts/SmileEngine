/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "text/string_view.h"

#include <string>

namespace smile::primitive
{
    // TODO: Move to string class
    std::string WideStringToString( const std::wstring &wideString );
    std::wstring StringToWideString( const std::string &str );

    std::optional< int > ToInt( StringView text );
    std::optional< float > ToFloat( StringView text );
}
