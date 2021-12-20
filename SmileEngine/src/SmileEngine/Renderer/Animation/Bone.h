#pragma once

namespace Smile
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
        Bone(const std::string& name, uint32_t id);
        void OnUpdate(float animationTime);

        const DirectX::XMFLOAT4X4& GetLocalTransform() const { return m_LocalTransform; }
        const std::string& GetName() const { return m_Name; }

    private:
        void InterpolateTranslation(DirectX::XMFLOAT3& position, float animationTime);
        uint32_t GetTranslationIndex(float animationTime);

        void InterpolateRotation(DirectX::XMFLOAT4& rotation, float animationTime);
        uint32_t GetRotationIndex(float animationTime);

        void InterpolateScale(DirectX::XMFLOAT3& scale, float animationTime);
        uint32_t GetScaleIndex(float animationTime);

        float GetScaleFactor(float lastTick, float nextTick, float animationTime);

    private:
        std::vector<KeyTranslation> m_Translations{};
        std::vector<KeyRotation> m_Rotations{};
        std::vector<KeyScale> m_Scales{};
        uint32_t m_TranslationCount{};
        uint32_t m_RotationCount{};
        uint32_t m_ScaleCount{};

        DirectX::XMFLOAT4X4 m_LocalTransform{};
        std::string m_Name;
        uint32_t m_ID;

        friend class MeshLoader;
    };
}
