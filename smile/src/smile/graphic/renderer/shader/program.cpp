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
 * @file        program.cpp
 * @author      Zenn Geeraerts
 * @created     2 February 2026
 * @brief       Unified shader program interface
 */
#include "smpch.h"
#include "program.h"

namespace smile::graphic
{
    static bool ValidateReflectionData( const ShaderReflectionData &vsReflection,
        const ShaderReflectionData &psReflection )
    {
        if ( vsReflection.TargetProfile != psReflection.TargetProfile ||
             vsReflection.BlobFormat != psReflection.BlobFormat )
        {
            return false;
        }

        const bool validSignature = std::all_of( psReflection.InputSignature.begin(),
            psReflection.InputSignature.end(),
            [&vsReflection]( const rhi::BufferElement &psInputElement )
            { return vsReflection.OutputSignature.ContainsElement( psInputElement ); } );

        if ( !validSignature )
        {
            return false;
        }

        const bool validResourceBindings = std::all_of( vsReflection.ShaderResourceBindings.begin(),
            vsReflection.ShaderResourceBindings.end(),
            [&]( const auto &vsKv )
            {
                const auto &vsElem = vsKv.Value;

                auto it = psReflection.ShaderResourceBindings.FindItemAtKey( vsKv.Key );
                if ( it != psReflection.ShaderResourceBindings.end() )
                {
                    const auto &psElem = it.GetItem();

                    if ( vsElem.Element.Type == rhi::ResourceType::ConstantBuffer )
                    {
                        const auto &vsCB = vsReflection.ConstantBufferDescs[vsElem.Name];
                        const auto &psCB = psReflection.ConstantBufferDescs[psElem.Name];

                        return vsCB == psCB;
                    }
                }

                return true;
            } );

        if ( !validResourceBindings )
        {
            return false;
        }

        return true;
    }

    static void MergeReflectionData( const ShaderReflectionData &vsReflection,
        const ShaderReflectionData &psReflection,
        primitive::Vector< Program::Resource > &resources,
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > &constantBufferDescs )
    {
        primitive::HashMap< ResourceBindingKey, Program::Resource > merged;

        auto mergeShader = [&]( const ShaderReflectionData &reflectionData, rhi::ShaderStage stage )
        {
            for ( const auto &kv : reflectionData.ShaderResourceBindings )
            {
                const ResourceBindingKey &key = kv.Key;
                const NamedBindingLayoutElement &elem = kv.Value;

                auto it = merged.FindItemAtKey( key );
                if ( it != merged.end() )
                {
                    it.GetItem().Visibility.Set( stage );
                }
                else
                {
                    Program::Resource resource{ elem.Element, elem.Name, { stage } };
                    merged.Insert( key, std::move( resource ) );

                    if ( elem.Element.Type == rhi::ResourceType::ConstantBuffer )
                    {
                        const auto &cbDesc = reflectionData.ConstantBufferDescs.GetItemAtKey( elem.Name );
                        constantBufferDescs.Insert( elem.Name, cbDesc );
                    }
                }
            }
        };

        mergeShader( vsReflection, rhi::ShaderStage::Vertex );
        mergeShader( psReflection, rhi::ShaderStage::Pixel );

        resources.Clear();
        resources.SetItemCount( merged.GetItemCount() );
        for ( const auto &kv : merged )
        {
            resources.PushBack( kv.Value );
        }
    }

    Program::Ref Program::Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader )
    {
        SM_ASSERT( vertexShader->GetReflectionData() );
        SM_ASSERT( pixelShader->GetReflectionData() );

        const ShaderReflectionData &vsReflection = *vertexShader->GetReflectionData();
        const ShaderReflectionData &psReflection = *pixelShader->GetReflectionData();

        SM_ASSERT( ValidateReflectionData( vsReflection, psReflection ) );

        primitive::Vector< Program::Resource > resources;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > cbDescs;
        MergeReflectionData( vsReflection, psReflection, resources, cbDescs );

        return memory::CreateRef< Program >( vertexShader, pixelShader, resources, cbDescs );
    }
}