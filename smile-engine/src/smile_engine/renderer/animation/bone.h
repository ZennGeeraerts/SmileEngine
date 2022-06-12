#pragma once

namespace smile::renderer
{
    struct KeyTranslation final
    {
        DirectX::XMFLOAT3 translation{};
        float tick{};
    };

    struct KeyRotation final
    {
        DirectX::XMFLOAT4 rotation{};
        float tick{};
    };

    struct KeyScale final
    {
        DirectX::XMFLOAT3 scale{};
        float tick{};
    };

    class Bone final
    {
      public:
        Bone( const std::string &name, Uint32 id );
        void onUpdate( float animation_time );

        const DirectX::XMFLOAT4X4 &getLocalTransform() const
        {
            return localTransform;
        }
        const std::string &getName() const
        {
            return name;
        }

      private:
        void interpolateTranslation( DirectX::XMFLOAT3 &position, float animation_time );
        Uint32 getTranslationIndex( float animatanimation_timeionTime );

        void interpolateRotation( DirectX::XMFLOAT4 &rotation, float animation_time );
        Uint32 getRotationIndex( float animation_time );

        void interpolateScale( DirectX::XMFLOAT3 &scale, float animation_time );
        Uint32 getScaleIndex( float animation_time );

        float getScaleFactor( float last_tick, float next_tick, float animation_time );

      private:
        std::vector< KeyTranslation > translations{};
        std::vector< KeyRotation > rotations{};
        std::vector< KeyScale > scales{};
        Uint32 translationCount{};
        Uint32 rotationCount{};
        Uint32 scaleCount{};

        DirectX::XMFLOAT4X4 localTransform{};
        std::string name;
        Uint32 id;

        friend class MeshLoader;
    };
}
