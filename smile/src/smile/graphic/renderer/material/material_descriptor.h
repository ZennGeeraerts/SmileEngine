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
 * @file        material_descriptor.h
 * @author      Zenn Geeraerts
 * @created     13 April 2026
 * @brief       Values for parameters and texture bindings of a material
 */
#pragma once

#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/graphic/rhi/resource/sampler.h"

#include <DirectXMath.h>

#include <variant>

namespace smile::graphic
{
    using MaterialParameterValue = std::
        variant< bool, int, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4, primitive::Vector< Byte > >;

    struct MaterialTextureBinding final
    {
        MaterialTextureBinding() = default;

        MaterialTextureBinding( const Texture &texture, const rhi::SamplerDescriptor &samplerDesc ) noexcept
            : Texture{ texture }, SamplerDescriptor{ samplerDesc }
        {
        }

        bool operator==( const MaterialTextureBinding &other ) const noexcept
        {
            return Texture == other.Texture && SamplerDescriptor == other.SamplerDescriptor;
        }

        bool operator!=( const MaterialTextureBinding &other ) const noexcept
        {
            return !( *this == other );
        }

        Texture Texture;
        rhi::SamplerDescriptor SamplerDescriptor;
    };

    struct MaterialDescriptor final
    {
        Program::ConstRef ShaderProgram;
        primitive::HashMap< primitive::String, MaterialParameterValue > Parameters;
        primitive::HashMap< primitive::String, MaterialTextureBinding > TextureBindings;
    };
}