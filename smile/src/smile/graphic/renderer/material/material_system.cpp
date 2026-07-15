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
 * @file        material_system.cpp
 * @author      Zenn Geeraerts
 * @created     11 May 2026
 * @brief       Public material system API
 */
#include "smpch.h"
#include "material_system.h"

#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/sprite/texture_manager.h"

namespace smile::graphic
{
    MaterialSystem::MaterialSystem( RenderContext &context,
        ResourceManager &resourceManager,
        TextureManager &textureManager ) noexcept
        : m_Internal{ context, resourceManager },
          m_ResourceManager{ resourceManager },
          m_TextureManager{ textureManager }
    {
    }

    MaterialDescriptor MaterialSystem::BuildMaterialDescriptor( const MaterialAssetDescriptor &assetDesc,
        const MaterialLayout &layout )
    {
        MaterialDescriptor desc;
        desc.ShaderProgram = assetDesc.ShaderProgram;
        desc.Parameters = assetDesc.Parameters;

        for ( const auto &textureLayout : layout.Textures )
        {
            const auto &binding = assetDesc.GetTextureBinding( textureLayout.Name );

            TextureAsset::ConstRef textureAsset = binding.Texture ? binding.Texture : m_TextureManager.GetFallback();
            const Texture texture = m_ResourceManager.GetOrCreateTexture2D( textureAsset );

            desc.TextureBindings.Insert( textureLayout.Name, { texture, binding.SamplerDescriptor } );
        }

        return desc;
    }
}