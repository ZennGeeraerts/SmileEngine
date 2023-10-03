float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float3 gColor : COLOR = float3( 1, 1, 1 );
Texture2D gTexture : DIFFUSE;
bool gUseTexture : USETEXTURE = false;

SamplerState gSamLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; // or Mirror or Clamp or Border
    AddressV = Wrap; // or Mirror or Clamp or Border
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input )
{
    float4x4 worldViewProjection = mul( gWorld, gViewProjection );

    VS_OUTPUT output = ( VS_OUTPUT )0;
    output.Position = mul( float4( input.Position, 1.f ), worldViewProjection );
    output.TexCoord = input.TexCoord;
    return output;
}

float4 PS( VS_OUTPUT input ) : SV_TARGET
{
    float3 color;

    if ( gUseTexture )
        color = gTexture.Sample( gSamLinear, input.TexCoord );
    else
        color = gColor;

    return float4( color, 1 );
}

technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}