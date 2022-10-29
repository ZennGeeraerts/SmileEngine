#pragma once

namespace Smile::Graphic
{
    struct KeyTranslation final
    {
        DirectX::XMFLOAT3 Translation{};
        float Tick{};
    };

    struct KeyRotation final
    {
        DirectX::XMFLOAT4 Rotation{};
        float Tick{};
    };

    struct KeyScale final
    {
        DirectX::XMFLOAT3 Scale{};
        float Tick{};
    };

    class Bone final
    {
      public:
        Bone( const std::string &name, Uint32 id );
        void OnUpdate( float animationTime );

        const DirectX::XMFLOAT4X4 &GetLocalTransform() const
        {
            return m_LocalTransform;
        }
        const std::string &GetName() const
        {
            return m_Name;
        }

      private:
        void InterpolateTranslation( DirectX::XMFLOAT3 &position, float animationTime );
        Uint32 GetTranslationIndex( float animationTime );

        void InterpolateRotation( DirectX::XMFLOAT4 &rotation, float animationTime );
        Uint32 GetRotationIndex( float animationTime );

        void InterpolateScale( DirectX::XMFLOAT3 &scale, float animationTime );
        Uint32 GetScaleIndex( float animationTime );

        float GetScaleFactor( float lastTick, float nextTick, float animationTime );

      private:
        std::vector< KeyTranslation > m_Translations{};
        std::vector< KeyRotation > m_Rotations{};
        std::vector< KeyScale > m_Scales{};
        Uint32 m_TranslationCount{};
        Uint32 m_RotationCount{};
        Uint32 m_ScaleCount{};

        DirectX::XMFLOAT4X4 m_LocalTransform{};
        std::string m_Name;
        Uint32 m_ID;

        friend class MeshLoader;
    };
}
