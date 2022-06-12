#include "smpch.h"
#include "bone.h"
#include "smile_engine/core/logger.h"

namespace smile::renderer
{
    Bone::Bone( const std::string &name, Uint32 id ) : name{ name }, id{ id }
    {
        DirectX::XMStoreFloat4x4( &localTransform, DirectX::XMMatrixIdentity() );
    }

    void Bone::onUpdate( float animation_time )
    {
        DirectX::XMFLOAT3 translation{};
        interpolateTranslation( translation, animation_time );
        DirectX::XMFLOAT4 rotation{};
        interpolateRotation( rotation, animation_time );
        DirectX::XMFLOAT3 scale{};
        interpolateScale( scale, animation_time );

        DirectX::XMMATRIX local_transform_mat = DirectX::XMMatrixTransformation( DirectX::XMVectorZero(),
            DirectX::XMQuaternionIdentity(),
            DirectX::XMLoadFloat3( &scale ),
            DirectX::XMVectorZero(),
            DirectX::XMLoadFloat4( &rotation ),
            DirectX::XMLoadFloat3( &translation ) );

        DirectX::XMStoreFloat4x4( &localTransform, local_transform_mat );
    }

    void Bone::interpolateTranslation( DirectX::XMFLOAT3 &translation, float animation_time )
    {
        if ( translationCount == 0 )
        {
            translation = DirectX::XMFLOAT3{ 0, 0, 0 };
            return;
        }

        if ( translationCount == 1 )
        {
            translation = {
                translations[0].translation.x, translations[0].translation.y, translations[0].translation.z };
            return;
        }

        Uint32 p0_index = getTranslationIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = getScaleFactor( translations[p0_index].tick, translations[p1_index].tick, animation_time );

        DirectX::XMVECTOR final_translation_vec =
            DirectX::XMVectorLerp( DirectX::XMLoadFloat3( &translations[p0_index].translation ),
                DirectX::XMLoadFloat3( &translations[p1_index].translation ),
                scale_factor );
        DirectX::XMStoreFloat3( &translation, final_translation_vec );
    }

    Uint32 Bone::getTranslationIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( translationCount - 1 ); ++i )
        {
            if ( animation_time <= translations[static_cast< size_t >( i ) + 1].tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getTranslationIndex > Translation index is 0" );
        return 0;
    }

    void Bone::interpolateRotation( DirectX::XMFLOAT4 &rotation, float animation_time )
    {
        if ( rotationCount == 0 )
        {
            DirectX::XMStoreFloat4( &rotation, DirectX::XMQuaternionIdentity() );
            return;
        }

        if ( rotationCount == 1 )
        {
            DirectX::XMVECTOR rotation_vec =
                DirectX::XMQuaternionNormalize( DirectX::XMLoadFloat4( &rotations[0].rotation ) );
            DirectX::XMStoreFloat4( &rotation, rotation_vec );
            return;
        }

        Uint32 p0_index = getRotationIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = getScaleFactor( rotations[p0_index].tick, rotations[p1_index].tick, animation_time );

        DirectX::XMVECTOR finalRotationVec =
            DirectX::XMQuaternionSlerp( DirectX::XMLoadFloat4( &rotations[p0_index].rotation ),
                DirectX::XMLoadFloat4( &rotations[p1_index].rotation ),
                scale_factor );
        finalRotationVec = DirectX::XMQuaternionNormalize( finalRotationVec );
        DirectX::XMStoreFloat4( &rotation, finalRotationVec );
    }

    Uint32 Bone::getRotationIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( rotationCount - 1 ); ++i )
        {
            if ( animation_time <= rotations[static_cast< size_t >( i ) + 1].tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getRotationIndex > Rotation index is 0" );
        return 0;
    }

    void Bone::interpolateScale( DirectX::XMFLOAT3 &scale, float animation_time )
    {
        if ( scaleCount == 0 )
        {
            scale = DirectX::XMFLOAT3{ 1, 1, 1 };
            return;
        }

        if ( scaleCount == 1 )
        {
            scale = { scales[0].scale.x, scales[0].scale.y, scales[0].scale.z };
            return;
        }

        Uint32 p0_index = getScaleIndex( animation_time );
        Uint32 p1_index = p0_index + 1;
        float scale_factor = getScaleFactor( scales[p0_index].tick, scales[p1_index].tick, animation_time );
        DirectX::XMVECTOR final_scale_vec = DirectX::XMVectorLerp( DirectX::XMLoadFloat3( &scales[p0_index].scale ),
            DirectX::XMLoadFloat3( &scales[p1_index].scale ),
            scale_factor );
        DirectX::XMStoreFloat3( &scale, final_scale_vec );
    }

    Uint32 Bone::getScaleIndex( float animation_time )
    {
        for ( Uint32 i{}; i < ( scaleCount - 1 ); ++i )
        {
            if ( animation_time <= scales[static_cast< size_t >( i ) + 1].tick )
                return i;
        }

        SM_ASSERT( false, "Bone::getScaleIndex > Scale index is 0" );
        return 0;
    }

    float Bone::getScaleFactor( float last_tick, float next_tick, float animation_time )
    {
        float mid_way_length = animation_time - last_tick;
        float frame_difference = next_tick - last_tick;
        return mid_way_length / frame_difference;
    }
}