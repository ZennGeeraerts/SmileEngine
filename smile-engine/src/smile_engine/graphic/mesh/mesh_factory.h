/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "static_mesh_filter.h"

namespace smile::graphic
{
    class MeshFactory final
    {
      public:
        static Ref< StaticMeshFilter > CreateCube( const BufferLayout &bufferLayout );
    };
}