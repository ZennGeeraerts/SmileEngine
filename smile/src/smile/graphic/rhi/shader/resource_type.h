/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string_view.h"

namespace smile::graphic::rhi
{
    enum class ResourceType : Uint8
    {
        Unknown,
        Texture_SRV,
        Texture_UAV,
        TypedBuffer_SRV,
        TypedBuffer_UAV,
        StructuredBuffer_SRV,
        StructuredBuffer_UAV,
        RawBuffer_SRV,
        RawBuffer_UAV,
        ConstantBuffer,
        VolatileConstantBuffer,
        Sampler,
        RayTracingAccelStruct,
        PushConstants,
        SamplerFeedbackTexture_UAV,

        Count
    };

    struct ResourceTypeInfo final
    {
        ResourceType Type;
        primitive::StringView Name;
    };

    const ResourceTypeInfo &GetResourceTypeInfo( ResourceType resourceType );
    const ResourceTypeInfo &GetResourceTypeInfo( const primitive::StringView resourceTypeView );
}