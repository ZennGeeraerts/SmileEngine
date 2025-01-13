float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

Texture2D gDiffuseMap : AlbedoMap;
SamplerState SamLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; // or Mirror or Clamp or Border
    AddressV = Wrap; // or Mirror or Clamp or Border
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float4x4 worldViewProjection = mul(gWorld, gViewProjection);
    
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.f), worldViewProjection);
    output.Normal = normalize(mul(input.Normal, (float3x3)gWorld));
    output.TexCoord = input.TexCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuseColor = gDiffuseMap.Sample(SamLinear, input.TexCoord);
	
	// HalfLambert Diffuse
    float diffuseStrength = dot(input.Normal, -gLightDirection);
    diffuseStrength = diffuseStrength * 0.5 + 0.5;
    diffuseStrength = saturate(diffuseStrength);
    diffuseColor.rgb = diffuseColor.rgb * diffuseStrength;
	
    return float4(diffuseColor);
}

technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}