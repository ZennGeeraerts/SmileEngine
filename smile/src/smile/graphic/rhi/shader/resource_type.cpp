/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "resource_type.h"

namespace smile::graphic::rhi
{
    static const ResourceTypeInfo s_ResourceTypeInfo[] = { { ResourceType::Unknown, "Unknown" },
        { ResourceType::Texture_SRV, "Texture_SRV" },
        { ResourceType::Texture_UAV, "Texture_UAV" },
        { ResourceType::TypedBuffer_SRV, "TypedBuffer_SRV" },
        { ResourceType::TypedBuffer_UAV, "TypedBuffer_UAV" },
        { ResourceType::StructuredBuffer_SRV, "StructuredBuffer_SRV" },
        { ResourceType::StructuredBuffer_UAV, "StructuredBuffer_UAV" },
        { ResourceType::RawBuffer_SRV, "RawBuffer_SRV" },
        { ResourceType::RawBuffer_UAV, "RawBuffer_UAV" },
        { ResourceType::ConstantBuffer, "ConstantBuffer" },
        { ResourceType::VolatileConstantBuffer, "VolatileConstantBuffer" },
        { ResourceType::Sampler, "Sampler" },
        { ResourceType::RayTracingAccelStruct, "RayTracingAccelStruct" },
        { ResourceType::PushConstants, "PushConstants" },
        { ResourceType::SamplerFeedbackTexture_UAV, "SamplerFeedbackTexture_UAV" } };

    const ResourceTypeInfo &GetResourceTypeInfo( ResourceType resourceType )
    {
        static_assert(
            sizeof( s_ResourceTypeInfo ) / sizeof( ResourceTypeInfo ) == static_cast< size_t >( ResourceType::Count ) );

        if ( static_cast< Uint32 >( resourceType ) >= static_cast< Uint32 >( ResourceType::Count ) )
            return s_ResourceTypeInfo[0]; // return unknown

        const ResourceTypeInfo &info = s_ResourceTypeInfo[static_cast< Uint32 >( resourceType )];

        SM_ASSERT_MSG( info.Type == resourceType, "Resource type mismatch" );

        return info;
    }

    const ResourceTypeInfo &GetResourceTypeInfo( const primitive::StringView resourceTypeView )
    {
        for ( const ResourceTypeInfo &info : s_ResourceTypeInfo )
        {
            if ( resourceTypeView == info.Name )
            {
                return info;
            }
        }

        return s_ResourceTypeInfo[0]; // return unknown
    }
}