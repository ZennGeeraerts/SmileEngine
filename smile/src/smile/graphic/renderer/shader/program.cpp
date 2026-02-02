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
                    const auto &vsElemnt = kv.Value;
                    const auto &psElement = it.GetItem();

                    return vsElemnt == psElement;
                }

                return true;
            } );

        if ( !validResourceBindings )
        {
            return false;
        }

        return true;
    }

    Program Program::Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader )
    {
        SM_ASSERT( vertexShader->GetReflectionData() );
        SM_ASSERT( pixelShader->GetReflectionData() );

        const ShaderReflectionData &vsReflection = *vertexShader->GetReflectionData();
        const ShaderReflectionData &psReflection = *pixelShader->GetReflectionData();
        
        SM_ASSERT( ValidateReflectionData( vsReflection, psReflection ) );
    }
}