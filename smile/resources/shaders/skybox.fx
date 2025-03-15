SamplerState gSamLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

TextureCube gCubeMap : CubeMap;
float4x4 gWorld : World;
float4x4 gViewProjection : ViewProjection;

struct VS_IN
{
    float3 posL : POSITION;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float3 texC : TEXCOORD;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    float4x4 worldViewProjection = mul(gWorld, gViewProjection);
    
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane)
	// Use local vertex position as cubemap lookup vector
    output.posH = mul(float4(input.posL, 0.0f), worldViewProjection).xyww;
    output.texC = input.posL;

    return output;
}

float4 PS(VS_OUT input) : SV_Target
{
    return gCubeMap.Sample(gSamLinear, input.texC);
}

technique11 Render
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}