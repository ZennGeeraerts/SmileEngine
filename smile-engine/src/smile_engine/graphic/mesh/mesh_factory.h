#pragma once

#include "static_mesh_filter.h"

namespace Smile::Graphic
{
    class MeshFactory final
    {
      public:
        static Ref< StaticMeshFilter > CreateCube( const BufferLayout &bufferLayout );
    };
}