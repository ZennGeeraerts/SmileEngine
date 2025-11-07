/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/array_view.h"

namespace smile::commandline
{
    bool Parse( primitive::ArrayView< const char * > args );
}