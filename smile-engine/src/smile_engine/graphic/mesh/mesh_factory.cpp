#include "smpch.h"
#include "mesh_factory.h"

namespace smile::graphic
{
    static std::vector< DirectX::XMFLOAT3 > cubePositions
    { 
        // front
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },

        // back
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },

        // up
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },

        // down
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },

        // right
        { 1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, 1.0f },

        // left
        { -1.0f, 1.0f, -1.0f },
        { -1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f } 
    };

    static const std::vector< Uint32 > cubeIndices
    {
        // front
        0 + 0,
        1 + 0,
        2 + 0,
        2 + 0,
        1 + 0,
        3 + 0,
        // back
        0 + 4,
        2 + 4,
        1 + 4,
        1 + 4,
        2 + 4,
        3 + 4,
        // up
        0 + 8,
        1 + 8,
        2 + 8,
        2 + 8,
        1 + 8,
        3 + 8,
        // down
        0 + 12,
        2 + 12,
        1 + 12,
        1 + 12,
        2 + 12,
        3 + 12,
        // right
        0 + 16,
        1 + 16,
        2 + 16,
        2 + 16,
        1 + 16,
        3 + 16,
        // left
        0 + 20,
        2 + 20,
        1 + 20,
        1 + 20,
        2 + 20,
        3 + 20,
    };

    static std::vector< DirectX::XMFLOAT3 > cubeNormals
    { 
        // front
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },

        // back
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },

        // up
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },

        // down
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },

        // right
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },

        // left
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f } 
    };

    Ref< StaticMeshFilter > MeshFactory::createCube( const BufferLayout &buffer_layout )
    {
        Ref< StaticMeshFilter > static_mesh_filter = createRef< StaticMeshFilter >();

        for ( const auto &element : buffer_layout )
        {
            if ( element.name == "POSITION" )
            {
                static_mesh_filter->usePositions = true;
                static_mesh_filter->positions = cubePositions;
            }
            else if ( element.name == "NORMAL" )
            {
                static_mesh_filter->useNormals = true;
                static_mesh_filter->normals = cubeNormals;
            }
            /*else if (element.Name == "TexCoord")
            {
                pStaticMeshFilter->m_bUseTexCoords = true;
                pStaticMeshFilter->m_TexCoords = m_CubeTexCoords;
            }*/
        }

        static_mesh_filter->vertexCount = static_cast< Uint32 >( cubePositions.size() );
        static_mesh_filter->indices = cubeIndices;

        return static_mesh_filter;
    }
}