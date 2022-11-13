#include "smpch.h"
#include "mesh_factory.h"

namespace smile::graphic
{
    static std::vector< DirectX::XMFLOAT3 > s_CubePositions
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

    static const std::vector< Uint32 > s_CubeIndices
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

    static std::vector< DirectX::XMFLOAT3 > s_CubeNormals
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

    Ref< StaticMeshFilter > MeshFactory::CreateCube( const BufferLayout &bufferLayout )
    {
        Ref< StaticMeshFilter > static_mesh_filter = CreateRef< StaticMeshFilter >();

        for ( const auto &element : bufferLayout )
        {
            if ( element.Name == "POSITION" )
            {
                static_mesh_filter->m_UsePositions = true;
                static_mesh_filter->m_Positions = s_CubePositions;
            }
            else if ( element.Name == "NORMAL" )
            {
                static_mesh_filter->m_UseNormals = true;
                static_mesh_filter->m_Normals = s_CubeNormals;
            }
            /*else if (element.Name == "TexCoord")
            {
                pStaticMeshFilter->m_bUseTexCoords = true;
                pStaticMeshFilter->m_TexCoords = m_CubeTexCoords;
            }*/
        }

        static_mesh_filter->m_VertexCount = static_cast< Uint32 >( s_CubePositions.size() );
        static_mesh_filter->m_Indices = s_CubeIndices;

        return static_mesh_filter;
    }
}