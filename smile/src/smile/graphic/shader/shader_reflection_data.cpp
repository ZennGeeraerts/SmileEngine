/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        shader_reflection_data.cpp
 * @author      Zenn Geeraerts
 * @created     9 February 2026
 * @brief       Reflected data from shader source files
 */
#include "smpch.h"
#include "shader_reflection_data.h"

namespace smile::graphic
{
    ResourceBindingType ResourceTypeToBindingType( rhi::ResourceType resType )
    {
        switch ( resType )
        {
            case rhi::ResourceType::Texture_SRV:
            case rhi::ResourceType::Texture_UAV:
            case rhi::ResourceType::SamplerFeedbackTexture_UAV:
                return ResourceBindingType::Texture;
            case rhi::ResourceType::TypedBuffer_SRV:
            case rhi::ResourceType::TypedBuffer_UAV:
            case rhi::ResourceType::StructuredBuffer_SRV:
            case rhi::ResourceType::StructuredBuffer_UAV:
            case rhi::ResourceType::RawBuffer_SRV:
            case rhi::ResourceType::RawBuffer_UAV:
            case rhi::ResourceType::ConstantBuffer:
            case rhi::ResourceType::VolatileConstantBuffer:
            case rhi::ResourceType::PushConstants:
                return ResourceBindingType::Buffer;
            case rhi::ResourceType::Sampler:
                return ResourceBindingType::Sampler;
        }

        return ResourceBindingType::Unknown;
    }
}