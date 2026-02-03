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
            [&psReflection]( const auto &kv )
            {
                auto it = psReflection.ShaderResourceBindings.FindItemAtKey( kv.Key );
                if ( it != psReflection.ShaderResourceBindings.end() )
                {
                    const auto &vsElement = kv.Value;
                    const auto &psElement = it.GetItem();

                    return vsElement == psElement;
                }

                return true;
            } );

        if ( !validResourceBindings )
        {
            return false;
        }

        return true;
    }

    template < typename Type >
    static void MergeData( primitive::HashMap< primitive::String, Type > &merged,
        const primitive::HashMap< primitive::String, Type > &toBeMerged )
    {
        for ( const auto &[key, value] : toBeMerged )
        {
            if ( !merged.HasItemAtKey( key ) )
            {
                merged.Insert( key, value );
            }
            else
            {
                [[maybe_unused]] const auto &existing = merged.GetItemAtKey( key );

                SM_ASSERT( existing == value );
            }
        }
    }

    static Program::ReflectionData MergeReflectionData( const ShaderReflectionData &vsReflection,
        const ShaderReflectionData &psReflection )
    {
        Program::ReflectionData merged{};

        merged.VertexLayout = vsReflection.InputSignature;

        primitive::HashMap< primitive::String, rhi::BindingLayoutElement > allBindings{
            vsReflection.ShaderResourceBindings };
        allBindings.InsertItems( psReflection.ShaderResourceBindings );

        for ( const auto &[key, value] : allBindings )
        {
            if ( merged.Bindings.TryAddElement( value ) )
            {
                merged.NameToBindingMap.Insert( key, value );
            }
            else
            {
                SM_ASSERT( false );
            }
        }

        primitive::HashMap< primitive::String, ConstantBufferDescriptor > allConstantBuffers{
            vsReflection.ConstantBufferDescs };
        allConstantBuffers.InsertItems( psReflection.ConstantBufferDescs );

        MergeData( merged.ConstantBufferDescs, allConstantBuffers );

        return merged;
    }

    Program::Ref Program::Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader )
    {
        SM_ASSERT( vertexShader->GetReflectionData() );
        SM_ASSERT( pixelShader->GetReflectionData() );

        const ShaderReflectionData &vsReflection = *vertexShader->GetReflectionData();
        const ShaderReflectionData &psReflection = *pixelShader->GetReflectionData();

        SM_ASSERT( ValidateReflectionData( vsReflection, psReflection ) );

        const ReflectionData reflectionData = MergeReflectionData( vsReflection, psReflection );

        return memory::CreateRef< Program >( vertexShader, pixelShader, reflectionData );
    }
}