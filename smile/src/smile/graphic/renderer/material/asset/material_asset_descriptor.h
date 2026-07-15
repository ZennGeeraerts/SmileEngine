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
 * @file        material_asset_descriptor.h
 * @author      Zenn Geeraerts
 * @created     11 May 2026
 * @brief       Asset-level material descriptor; holds TextureAsset refs instead of GPU resources
 */
#pragma once

#include "smile/graphic/renderer/material/material_parameter_value.h"
#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/renderer/sprite/texture_asset.h"
#include "smile/graphic/rhi/resource/sampler.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    struct MaterialAssetTextureBinding final
    {
        MaterialAssetTextureBinding() = default;

        MaterialAssetTextureBinding( TextureAsset::Ref texture, const rhi::SamplerDescriptor &samplerDesc ) noexcept
            : Texture{ texture }, SamplerDescriptor{ samplerDesc }
        {
        }

        TextureAsset::Ref Texture;
        rhi::SamplerDescriptor SamplerDescriptor;
    };

    struct MaterialAssetDescriptor final
    {
        void SetParameter( const primitive::StringView name, const MaterialParameterValue &data );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            TextureAsset::Ref textureAsset,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialAssetTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        Program::ConstRef ShaderProgram;
        primitive::HashMap< primitive::String, MaterialParameterValue > Parameters;
        primitive::HashMap< primitive::String, MaterialAssetTextureBinding > TextureBindings;
    };
}
