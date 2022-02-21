float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float4x4 gViewInverse : VIEWINVERSE;

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float4x4 worldViewProjection = mul(gWorld, gViewProjection);
    
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.f), worldViewProjection);
    output.Color = input.Color;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return float4(input.Color, 1);
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