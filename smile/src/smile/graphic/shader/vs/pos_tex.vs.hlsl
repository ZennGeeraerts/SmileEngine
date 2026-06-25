/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Camera : register( b0 )
{
    float4x4 ViewProjection;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;

    float4 Row0 : INSTANCE_ROW0;
    float4 Row1 : INSTANCE_ROW1;
    float4 Row2 : INSTANCE_ROW2;
    float4 Row3 : INSTANCE_ROW3;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VSMain( VS_INPUT input )
{
    float4x4 worldTransform = float4x4( input.Row0, input.Row1, input.Row2, input.Row3 );
    float4 worldPos = mul(worldTransform, float4(input.Position, 1.0f));

    VS_OUTPUT output = ( VS_OUTPUT )0;
    output.Position = mul(worldPos, ViewProjection);
    output.TexCoord = input.TexCoord;
    return output;
}
