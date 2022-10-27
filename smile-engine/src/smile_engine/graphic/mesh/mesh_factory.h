#pragma once

#include "static_mesh_filter.h"

namespace smile::graphic
{
    class MeshFactory final
    {
      public:
        static Ref< StaticMeshFilter > createCube( const BufferLayout &buffer_layout );
    };
}