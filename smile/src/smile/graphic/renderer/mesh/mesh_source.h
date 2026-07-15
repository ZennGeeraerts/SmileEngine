/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/flags.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    enum class Semantic : Uint32
    {
        None = 0,
        Positon,
        Normal,
        TexCoord,
        Binormal,
        Tangent,
        Color,
        BlendIndices,
        BlendWeights
    };

    class MeshSource : public memory::Counted
    {
      public:
        using Ref = memory::Ref< MeshSource >;
        using ConstRef = memory::Ref< const MeshSource >;

        MeshSource() = default;
        MeshSource( const primitive::String &name ) noexcept : m_Name{ name }
        {
        }

        virtual ~MeshSource() = default;

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

        void SetVertexCount( Count vertexCount )
        {
            m_VertexCount = vertexCount;
        }

        void AddPosition( const DirectX::XMFLOAT3 &position );
        void AddNormal( const DirectX::XMFLOAT3 &normal );
        void AddTangent( const DirectX::XMFLOAT3 &tangent );
        void AddBinormal( const DirectX::XMFLOAT3 &binormal );
        void AddTexCoord( const DirectX::XMFLOAT2 &texCoord );
        void AddColor( const DirectX::XMFLOAT4 &color );

        void SetIndexCount( Count indexCount );
        void AddIndex( Index bufferPosition, Index index );

        bool HasSemantic( Semantic semantic )
        {
            return m_Semantics.Has( semantic );
        }

      private:
        primitive::String m_Name;
        primitive::Vector< DirectX::XMFLOAT3 > m_Positions = {};
        primitive::Vector< DirectX::XMFLOAT3 > m_Normals = {};
        primitive::Vector< DirectX::XMFLOAT3 > m_Tangents = {};
        primitive::Vector< DirectX::XMFLOAT3 > m_Binormals = {};
        primitive::Vector< DirectX::XMFLOAT2 > m_TexCoords = {};
        primitive::Vector< DirectX::XMFLOAT4 > m_Colors = {};

        foundation::Flags< Semantic > m_Semantics{ Semantic::None };

        primitive::Vector< Uint32 > m_Indices = {};

        Count m_VertexCount = 0;

        void *m_pDataLocation = nullptr;

        friend class MeshFactory;
        friend class ModelLoader;
    };
}