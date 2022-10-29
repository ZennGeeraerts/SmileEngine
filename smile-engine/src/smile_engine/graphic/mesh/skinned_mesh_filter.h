#pragma once
#include "mesh_filter.h"
#include "smile_engine/graphic/animation/bone.h"

namespace Smile::Graphic
{
    struct BoneInfo final
    {
        Uint32 id{};
        DirectX::XMFLOAT4X4 offset{};
    };

    struct AnimationNode final
    {
        DirectX::XMFLOAT4X4 transform{};
        std::string name{};
        Uint32 childrenCount{};
        std::vector< AnimationNode > children;
    };

    struct AnimationClip final
    {
        std::string name{};
        float duration{};
        float ticksPerSecond{};
        std::vector< Bone > bones{};
        AnimationNode rootNode{};
    };

    class SkinnedMeshFilter final : public MeshFilter
    {
      public:
        SkinnedMeshFilter() = default;
        virtual ~SkinnedMeshFilter();

        virtual void Create( const BufferLayout &layout ) override;
        bool HasAnimations() const
        {
            return m_IsAnimated;
        }

        virtual Ref< VertexBuffer > GetVertexBuffer() const override
        {
            return m_pVertexBuffer;
        }
        virtual Ref< IndexBuffer > GetIndexBuffer() const override
        {
            return m_pIndexBuffer;
        }
        virtual const std::string &GetFilePath() const override
        {
            return m_FilePath;
        }

        virtual void SetVertexCount( Uint32 vertex_count ) override
        {
            m_VertexCount = vertex_count;
        }
        virtual void AddPosition( const DirectX::XMFLOAT3 &position ) override;
        virtual void AddNormal( const DirectX::XMFLOAT3 &normal ) override;
        virtual void AddTangent( const DirectX::XMFLOAT3 &tangent ) override;
        virtual void AddBinormal( const DirectX::XMFLOAT3 &binormal ) override;
        virtual void AddTexCoord( const DirectX::XMFLOAT2 &texCoord ) override;
        virtual void AddColor( const DirectX::XMFLOAT4 &color ) override;

        virtual void SetIndexCount( Uint32 indexCount ) override;
        virtual void AddIndex( Uint32 bufferPosition, Uint32 index ) override;

      private:
        std::vector< DirectX::XMFLOAT3 > m_Positions = {};
        std::vector< DirectX::XMFLOAT3 > m_Normals = {};
        std::vector< DirectX::XMFLOAT3 > m_Tangents = {};
        std::vector< DirectX::XMFLOAT3 > m_Binormals = {};
        std::vector< DirectX::XMFLOAT2 > m_TexCoords = {};
        std::vector< DirectX::XMFLOAT4 > m_Colors = {};
        std::vector< DirectX::XMFLOAT4 > m_BlendIndices = {};
        std::vector< DirectX::XMFLOAT4 > m_BlendWeights = {};

        bool m_IsAnimated = false;
        std::vector< AnimationClip > m_AnimationClips{};
        std::unordered_map< std::string, BoneInfo > m_SkeletonMap{};
        Uint32 m_BoneCount = 0;

        bool m_UsePositions = false;
        bool m_UseNormals = false;
        bool m_UseTangents = false;
        bool m_UseBinormals = false;
        bool m_UseTexCoords = false;
        bool m_UseColors = false;
        bool m_UseBlendIndices = false;
        bool m_UseBlendWeights = false;

        std::vector< Uint32 > m_Indices = {};

        Ref< VertexBuffer > m_pVertexBuffer = nullptr;
        Ref< IndexBuffer > m_pIndexBuffer = nullptr;

        Uint32 m_VertexCount = 0;

        std::string m_FilePath = {};
        void *m_pDataLocation = nullptr;

        friend class MeshLoader;
        friend class MeshAnimator;
    };
}
