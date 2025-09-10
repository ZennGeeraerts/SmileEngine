/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Material : register( b0 )
{
    float3 Color;
    int UseTexture;
};

Texture Diffuse : register( t0 );
Sampler SamLinear : register( s0 );

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

float4 PSMain( VS_OUTPUT input ) : SV_TARGET
{
    float3 color;

    if ( UseTexture != 0 )
        color = Diffuse.Sample( SamLinear, input.TexCoord ).rgb;
    else
        color = Color;

    return float4( color, 1 );
}
