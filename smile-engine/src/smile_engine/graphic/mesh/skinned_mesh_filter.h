#pragma once
#include "mesh_filter.h"
#include "smile_engine/graphic/animation/bone.h"

namespace smile::graphic
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

        virtual void create( const BufferLayout &layout ) override;
        bool hasAnimations() const
        {
            return animated;
        }

        virtual Ref< VertexBuffer > getVertexBuffer() const override
        {
            return vertexBuffer;
        }
        virtual Ref< IndexBuffer > getIndexBuffer() const override
        {
            return indexBuffer;
        }
        virtual const std::string &getFilePath() const override
        {
            return filePath;
        }

        virtual void setVertexCount( Uint32 vertex_count ) override
        {
            vertexCount = vertex_count;
        }
        virtual void addPosition( const DirectX::XMFLOAT3 &position ) override;
        virtual void addNormal( const DirectX::XMFLOAT3 &normal ) override;
        virtual void addTangent( const DirectX::XMFLOAT3 &tangent ) override;
        virtual void addBinormal( const DirectX::XMFLOAT3 &binormal ) override;
        virtual void addTexCoord( const DirectX::XMFLOAT2 &tex_coord ) override;
        virtual void addColor( const DirectX::XMFLOAT4 &color ) override;

        virtual void setIndexCount( Uint32 index_count ) override;
        virtual void addIndex( Uint32 buffer_position, Uint32 index ) override;

      private:
        std::vector< DirectX::XMFLOAT3 > positions = {};
        std::vector< DirectX::XMFLOAT3 > normals = {};
        std::vector< DirectX::XMFLOAT3 > tangents = {};
        std::vector< DirectX::XMFLOAT3 > binormals = {};
        std::vector< DirectX::XMFLOAT2 > texCoords = {};
        std::vector< DirectX::XMFLOAT4 > colors = {};
        std::vector< DirectX::XMFLOAT4 > blendIndices = {};
        std::vector< DirectX::XMFLOAT4 > blendWeights = {};

        bool animated = false;
        std::vector< AnimationClip > animationClips{};
        std::unordered_map< std::string, BoneInfo > skeletonMap{};
        Uint32 boneCount = 0;

        bool usePositions = false;
        bool useNormals = false;
        bool useTangents = false;
        bool useBinormals = false;
        bool useTexCoords = false;
        bool useColors = false;
        bool useBlendIndices = false;
        bool useBlendWeights = false;

        std::vector< Uint32 > indices = {};

        Ref< VertexBuffer > vertexBuffer = nullptr;
        Ref< IndexBuffer > indexBuffer = nullptr;

        Uint32 vertexCount = 0;

        std::string filePath = {};
        void *dataLocation = nullptr;

        friend class MeshLoader;
        friend class MeshAnimator;
    };
}
