#pragma once
#include "mesh_filter.h"
#include "smile_engine/renderer/animation/bone.h"

namespace smile
{
    struct BoneInfo final
    {
        Uint32 m_ID{};
        DirectX::XMFLOAT4X4 m_Offset{};
    };

    struct AnimationNode final
    {
        DirectX::XMFLOAT4X4 m_Transform{};
        std::string m_Name{};
        Uint32 m_ChildrenCount{};
        std::vector< AnimationNode > m_Children;
    };

    struct AnimationClip final
    {
        std::string m_Name{};
        float m_Duration{};
        float m_TicksPerSecond{};
        std::vector< Bone > m_Bones{};
        AnimationNode m_RootNode{};
    };

    class SkinnedMeshFilter final : public MeshFilter
    {
      public:
        SkinnedMeshFilter() = default;
        virtual ~SkinnedMeshFilter();

        virtual void Create( const BufferLayout &layout ) override;
        bool HasAnimations() const
        {
            return m_bHasAnimations;
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

        virtual void SetVertexCount( Uint32 vertexCount ) override
        {
            m_VertexCount = vertexCount;
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

        bool m_bHasAnimations = false;
        std::vector< AnimationClip > m_AnimationClips{};
        std::unordered_map< std::string, BoneInfo > m_SkeletonMap{};
        uint32_t m_BoneCount = 0;

        bool m_bUsePositions = false;
        bool m_bUseNormals = false;
        bool m_bUseTangents = false;
        bool m_bUseBinormals = false;
        bool m_bUseTexCoords = false;
        bool m_bUseColors = false;
        bool m_bUseBlendIndices = false;
        bool m_bUseBlendWeights = false;

        std::vector< uint32_t > m_Indices = {};

        Ref< VertexBuffer > m_pVertexBuffer = nullptr;
        Ref< IndexBuffer > m_pIndexBuffer = nullptr;

        uint32_t m_VertexCount = 0;

        std::string m_FilePath = {};
        void *m_pDataLocation = nullptr;

        friend class MeshLoader;
        friend class MeshAnimator;
    };
}
