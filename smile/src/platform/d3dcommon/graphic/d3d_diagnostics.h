/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <Windows.h>

namespace smile::graphic::rhi
{
    LPTSTR GetD3DErrorMessage( HRESULT result );
}