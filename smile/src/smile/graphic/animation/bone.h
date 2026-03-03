/*=============================================================================*/
// Copyright 2022-20234 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"

#include <DirectXMath.h>

namespace smile::graphic
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
        Bone( const primitive::String &name, Index id );

        void OnUpdate( float animationTime );

        const DirectX::XMFLOAT4X4 &GetLocalTransform() const
        {
            return m_LocalTransform;
        }

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

      private:
        void InterpolateTranslation( DirectX::XMFLOAT3 &position, float animationTime );
        Index GetTranslationIndex( float animationTime );

        void InterpolateRotation( DirectX::XMFLOAT4 &rotation, float animationTime );
        Index GetRotationIndex( float animationTime );

        void InterpolateScale( DirectX::XMFLOAT3 &scale, float animationTime );
        Index GetScaleIndex( float animationTime );

        float GetScaleFactor( float lastTick, float nextTick, float animationTime );

      private:
        primitive::Vector< KeyTranslation > m_Translations{};
        primitive::Vector< KeyRotation > m_Rotations{};
        primitive::Vector< KeyScale > m_Scales{};
        Count m_TranslationCount{};
        Count m_RotationCount{};
        Count m_ScaleCount{};

        DirectX::XMFLOAT4X4 m_LocalTransform{};
        primitive::String m_Name;
        Index m_ID;

        friend class ModelLoader;
    };
}
