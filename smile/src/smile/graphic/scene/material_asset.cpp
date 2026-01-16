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
 * @file        material_asset.h
 * @author      Zenn Geeraerts
 * @created     16 Januari 2026
 * @brief       Asset for material
 */
#include "smpch.h"
#include "material_asset.h"

namespace smile::graphic
{
    MaterialAsset::MaterialAsset( ShaderAsset::ConstRef pVertexShader, ShaderAsset::ConstRef pPixelShader ) noexcept
        : m_pVertexShader{ pVertexShader }, m_pPixelShader{ pPixelShader }
    {
        SetShaders( pVertexShader, pPixelShader );
    }

    void MaterialAsset::SetShaders( const ShaderAsset::ConstRef &pVertexShader,
        const ShaderAsset::ConstRef &pPixelShader )
    {
        m_Bindings.Clear();
        m_ConstantBufferDescs.Clear();

        m_pVertexShader = pVertexShader;
        m_pPixelShader = pPixelShader;

        auto addBindingLayoutElements = [&]( const ShaderReflectionData &reflectionData )
        {
            for ( const auto &binding : reflectionData.ShaderResourceBindings )
            {
                SM_ASSERT_MSG( !m_Bindings.HasItemAtKey( binding.Key ),
                    "Bindings already contain an element with the name: {}",
                    binding.Key );

                m_Bindings.Insert( binding.Key, binding.Value );

                if ( binding.Value.Type == rhi::ResourceType::ConstantBuffer )
                {
                    SM_ASSERT_MSG( !m_ConstantBufferDescs.HasItemAtKey( binding.Key ),
                        "Constant buffer descriptors already contain a descriptor with the name: {}",
                        binding.Key )

                    const auto &constantBufferDesc = reflectionData.ConstantBufferDescs.GetItemAtKey( binding.Key );
                    m_ConstantBufferDescs.Insert( binding.Key, constantBufferDesc );
                }
            }
        };

        addBindingLayoutElements( pVertexShader->GetReflectionData() );
        addBindingLayoutElements( pPixelShader->GetReflectionData() );
    }
}