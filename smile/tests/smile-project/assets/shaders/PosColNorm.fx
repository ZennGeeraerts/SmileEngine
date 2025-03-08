float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float3 gColor = float3(1, 1, 1);
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float4x4 worldViewProjection = mul(gWorld, gViewProjection);
    
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.f), worldViewProjection);
    output.Normal = normalize(mul(input.Normal, (float3x3)gWorld));
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 color = gColor;
	
	//HalfLambert Diffuse
    float diffuseStrength = dot(input.Normal, -gLightDirection);
    diffuseStrength = diffuseStrength * 0.5 + 0.5;
    diffuseStrength = saturate(diffuseStrength);
    color = color * diffuseStrength;
	
    return float4(color, 1);
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