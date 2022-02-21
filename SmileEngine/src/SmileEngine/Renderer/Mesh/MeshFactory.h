#pragma once

#include "StaticMeshFilter.h"

namespace smile
{
    class MeshFactory final
    {
      public:
        static Ref< StaticMeshFilter > CreateCube( const BufferLayout &bufferLayout );
    };
}