#pragma once

#include "static_mesh_filter.h"

namespace smile
{
    class MeshFactory final
    {
      public:
        static Ref< StaticMeshFilter > CreateCube( const BufferLayout &bufferLayout );
    };
}