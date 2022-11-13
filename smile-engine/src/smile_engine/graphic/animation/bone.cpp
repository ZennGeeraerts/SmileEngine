#include "smpch.h"
#include "bone.h"
#include "smile_engine/core/logger.h"

namespace smile::graphic
{
    Bone::Bone( const std::string &name, Uint32 id ) : m_Name{ name }, m_ID{ id }
    {
        DirectX::XMStoreFloat4x4( &m_LocalTransform, DirectX::XMMatrixIdentity() );
    }

    void Bone::OnUpdate( float animation_time )
    {
        DirectX::XMFLOAT3 translation{};
        InterpolateTranslation( translation, animation_time );
        DirectX::XMFLOAT4 rotation{};
        InterpolateRotation( rotation, animation_time );
        DirectX::XMFLOAT3 scale{};
        InterpolateScale( scale, animation_time );

        DirectX::XMMATRIX local_transform_mat = DirectX::XMMatrixTransformation( DirectX::XMVectorZero(),
            DirectX::XMQuaternionIdentity(),
            DirectX::XMLoadFloat3( &scale ),
            DirectX::XMVectorZero(),
            DirectX::XMLoadFloat4( &rotation ),
            DirectX::XMLoadFloat3( &translation ) );

        DirectX::XMStoreFloat4x4( &m_LocalTransform, local_transform_mat );
    }

    void Bone::InterpolateTranslation( DirectX::XMFLOAT3 &translation, float animation_time )
    {
        if ( m_TranslationCount == 0 )
        {
            translation = DirectX::XMFLOAT3{ 0, 0, 0 };
            return;
        }

        if ( m_TranslationCount == 1 )
        {
            translation = {
                m_Translations[0].Translation.x, m_Translations[0].Translation.y, m_Translations[0].Translation.z };
            return;
        }

        Uint32 p0_index = GetTranslationIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = GetScaleFactor( m_Translations[p0_index].Tick, m_Translations[p1_index].Tick, animation_time );

        DirectX::XMVECTOR final_translation_vec =
            DirectX::XMVectorLerp( DirectX::XMLoadFloat3( &m_Translations[p0_index].Translation ),
                DirectX::XMLoadFloat3( &m_Translations[p1_index].Translation ),
                scale_factor );
        DirectX::XMStoreFloat3( &translation, final_translation_vec );
    }

    Uint32 Bone::GetTranslationIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( m_TranslationCount - 1 ); ++i )
        {
            if ( animation_time <= m_Translations[static_cast< size_t >( i ) + 1].Tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getTranslationIndex > Translation index is 0" );
        return 0;
    }

    void Bone::InterpolateRotation( DirectX::XMFLOAT4 &rotation, float animation_time )
    {
        if ( m_RotationCount == 0 )
        {
            DirectX::XMStoreFloat4( &rotation, DirectX::XMQuaternionIdentity() );
            return;
        }

        if ( m_RotationCount == 1 )
        {
            DirectX::XMVECTOR rotation_vec =
                DirectX::XMQuaternionNormalize( DirectX::XMLoadFloat4( &m_Rotations[0].Rotation ) );
            DirectX::XMStoreFloat4( &rotation, rotation_vec );
            return;
        }

        Uint32 p0_index = GetRotationIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = GetScaleFactor( m_Rotations[p0_index].Tick, m_Rotations[p1_index].Tick, animation_time );

        DirectX::XMVECTOR finalRotationVec =
            DirectX::XMQuaternionSlerp( DirectX::XMLoadFloat4( &m_Rotations[p0_index].Rotation ),
                DirectX::XMLoadFloat4( &m_Rotations[p1_index].Rotation ),
                scale_factor );
        finalRotationVec = DirectX::XMQuaternionNormalize( finalRotationVec );
        DirectX::XMStoreFloat4( &rotation, finalRotationVec );
    }

    Uint32 Bone::GetRotationIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( m_RotationCount - 1 ); ++i )
        {
            if ( animation_time <= m_Rotations[static_cast< size_t >( i ) + 1].Tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getRotationIndex > Rotation index is 0" );
        return 0;
    }

    void Bone::InterpolateScale( DirectX::XMFLOAT3 &scale, float animation_time )
    {
        if ( m_ScaleCount == 0 )
        {
            scale = DirectX::XMFLOAT3{ 1, 1, 1 };
            return;
        }

        if ( m_ScaleCount == 1 )
        {
            scale = { m_Scales[0].Scale.x, m_Scales[0].Scale.y, m_Scales[0].Scale.z };
            return;
        }

        Uint32 p0_index = GetScaleIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = GetScaleFactor( m_Scales[p0_index].Tick, m_Scales[p1_index].Tick, animation_time );
        DirectX::XMVECTOR final_scale_vec = DirectX::XMVectorLerp( DirectX::XMLoadFloat3( &m_Scales[p0_index].Scale ),
            DirectX::XMLoadFloat3( &m_Scales[p1_index].Scale ),
            scale_factor );
        DirectX::XMStoreFloat3( &scale, final_scale_vec );
    }

    Uint32 Bone::GetScaleIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( m_ScaleCount - 1 ); ++i )
        {
            if ( animation_time <= m_Scales[static_cast< size_t >( i ) + 1].Tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getScaleIndex > Scale index is 0" );
        return 0;
    }

    float Bone::GetScaleFactor( float last_tick, float next_tick, float animation_time )
    {
        float mid_way_length = animation_time - last_tick;
        float frame_difference = next_tick - last_tick;
        return mid_way_length / frame_difference;
    }
}