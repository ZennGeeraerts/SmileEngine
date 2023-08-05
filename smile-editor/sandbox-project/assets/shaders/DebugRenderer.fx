cbuffer cbPerObject{
	float4x4 ViewProjectionMat : VIEWPROJECTION;
	float4x4 WorldMat : WORLD
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input){

	float4x4 worldViewProjection = mul( WorldMat, ViewProjectionMat );

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul( float4( input.Pos, 1 ), worldViewProjection );
	output.Color = input.Color;

	return output;
}

float4 PS(VS_OUTPUT input):SV_TARGET
{
	return input.Color;
}

technique11 Default
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
	}
}

