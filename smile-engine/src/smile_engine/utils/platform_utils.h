/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <string>

namespace smile::utils
{
    std::string OpenFile( const char *filter );
    std::string SaveFile( const char *filter );
}