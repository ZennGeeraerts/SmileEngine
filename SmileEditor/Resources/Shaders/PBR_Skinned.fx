float4x4 gWorld : WORLD;
float4x4 gViewProjection : VIEWPROJECTION;
float4x4 gViewInverse : VIEWINVERSE;

float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f) * -1;
float3 gLightColor = float3(1.0f, 1.0f, 1.0f);
float gLightIntensity = 3.f;

bool gUseAlbedoMap : USEALBEDOMAP = false;
float3 gAlbedoValue : ALBEDO = float3(0.0f, 0.0f, 0.0f);
Texture2D gAlbedoMap : ALBEDOMAP;

bool gUseNormalMap : USENORMALMAP = false;
Texture2D gNormalMap : NORMALMAP;

bool gUseMetalnessMap : USEMETALNESSMAP = false;
float gMetalnessValue : METALNESS = 0.0f;
Texture2D gMetalnessMap : METALNESSMAP;

bool gUseRoughnessMap : USEROUGHNESSMAP = false;
float gRoughnessValue : ROUGHNESS = 0.5f;
Texture2D gRoughnessMap : ROUGHNESSMAP;

bool gUseAOMap : USEAOMAP = false;
float gAOValue = 1.0f;
Texture2D gAOMap : AOMap;

float3 gAmbientColor = 1.f;

TextureCube gEnvironmentMap : ENVIRONMENTMAP;
float gReflectionStrength = 0.9f;
float gRefractionStrength = 0.1f;
float gRefractionIndex = 0.3000f;

float gPI = 3.141592f;
float gEpsilon = 0.0001f;

// Skinning
float4x4 gBones[70] : BONES;

SamplerState gSamLinear
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
    float3 Tangent : TANGENT;
    float4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLDPOSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
};

struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0;
    float4 Color1 : SV_TARGET1;
};

float TrowbridgeReitzGGX(float3 normal, float3 halfVector, float roughness)
{
    float roughnessSqr = roughness * roughness;
    float roughnessSqrSqr = roughnessSqr * roughnessSqr;
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float NdotHSqr = NdotH * NdotH;
    
    float numerator = roughnessSqrSqr;
    float denomerator = (NdotHSqr * (roughnessSqrSqr - 1.f) + 1.f);
    denomerator = gPI * denomerator * denomerator;
    
    return numerator / denomerator;
}

float3 Schlick(float cosTheta, float3 baseReflectivity)
{
    return baseReflectivity + (1 - baseReflectivity) * pow(clamp(1 - cosTheta, 0.0f, 1.0f), 5);
}

float3 SchlickGGX(float nDot, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    
    float numerator = nDot;
    float denomerator = nDot * (1.0f - k) + k;
    
    return numerator / denomerator;
}

float3 Smith(float3 normal, float3 viewDirection, float roughness)
{
    float NdotV = max(dot(normal, -viewDirection), 0.0f);
    float NdotL = max(dot(normal, gLightDirection), 0.0f);
    return SchlickGGX(NdotL, roughness) * SchlickGGX(NdotV, roughness);
}

VS_OUTPUT VS(VS_INPUT input)
{
    float4x4 worldViewProjection = mul(gWorld, gViewProjection);
    
    VS_OUTPUT output = (VS_OUTPUT)0;
    float4 originalPosition = float4(input.Position, 1);
    float4 transformedPosition = 0;
    float3 transformedNormal = 0;

	//Skinning Magic...
    for (int i = 0; i < 4; ++i)
    {
        if (input.BlendIndices[i] > -1)
        {
            transformedPosition += input.BlendWeights[i] * mul(originalPosition, gBones[input.BlendIndices[i]]);
            transformedNormal += input.BlendWeights[i] * mul(input.Normal, (float3x3) gBones[input.BlendIndices[i]]);
            transformedPosition.w = 1.f;
        }
    }
	
    transformedNormal = normalize(transformedNormal);
	
    if (transformedPosition.w > 0)
    {
        output.Position = mul(transformedPosition, worldViewProjection);
        output.WorldPosition = mul(transformedPosition, gWorld);
        output.Normal = normalize(mul(transformedNormal, (float3x3) gWorld));
    }
    else
    {
        output.Position = mul(float4(input.Position, 1.0f), worldViewProjection); //Non skinned position
        output.WorldPosition = mul(float4(input.Position, 1.0f), gWorld);
        output.Normal = normalize(mul(input.Normal, (float3x3) gWorld)); //Non skinned normal
    }
    
    output.TexCoord = input.TexCoord;
    output.Tangent = normalize(mul(input.Tangent, (float3x3) gWorld));
    return output;
}

PS_OUTPUT PS(VS_OUTPUT input) : SV_TARGET
{
    float3 viewDirection = normalize(input.WorldPosition.xyz - gViewInverse[3].xyz);
    
    // Albedo
    float3 albedo = gAlbedoValue;
    if (gUseAlbedoMap)
        albedo = pow(gAlbedoMap.Sample(gSamLinear, input.TexCoord).rgb, 2.2f);
    
    // Normal
    float3 bumpNormal = input.Normal;
    if (gUseNormalMap)
    {
        float3 binormal = cross(input.Normal, input.Tangent);
        float3x3 localAxis = float3x3(input.Tangent, binormal, input.Normal);
        float3 sampledNormal = gNormalMap.Sample(gSamLinear, input.TexCoord).rgb;
        sampledNormal = 2.f * sampledNormal - 1.f;
        bumpNormal = normalize(mul(sampledNormal, localAxis));
    }

    // AO
    float ao = gAOValue;
    if (gUseAOMap)
        ao = gAOMap.Sample(gSamLinear, input.TexCoord).r;
	
    // Roughness
    float roughness = gRoughnessValue;
    if (gUseRoughnessMap)
        roughness = gRoughnessMap.Sample(gSamLinear, input.TexCoord).r;
    
    // Metalness
    float metalness = gMetalnessValue;
    if (gUseMetalnessMap)
        metalness = gMetalnessMap.Sample(gSamLinear, input.TexCoord).r;
    
    // Cube environment
    float3 reflected = reflect(-viewDirection, bumpNormal);
    float3 refracted = refract(-viewDirection, bumpNormal, gRefractionIndex);
    float3 environment = gEnvironmentMap.Sample(gSamLinear, reflected) * gReflectionStrength + gEnvironmentMap.Sample(gSamLinear, refracted) * gRefractionStrength;
    environment = saturate(environment);
    environment = environment * (1 - roughness);
    
    float3 baseReflectivity = 0.04f;
    baseReflectivity = lerp(baseReflectivity, albedo, metalness);
    
    float3 radianceColor = lerp(gLightColor, albedo, metalness);
    float3 radiance = mul(radianceColor, gLightIntensity);
    
    // cooktorrence
    float3 outgoingRadiance = 0.0f;
    {
        float3 halfVector = normalize(-viewDirection + gLightDirection);
        
        float normalDistribution = TrowbridgeReitzGGX(bumpNormal, halfVector, roughness);
        float geometry = Smith(bumpNormal, viewDirection, roughness);
        float fresnel = Schlick(max(dot(halfVector, -viewDirection), 0.0f), baseReflectivity);
    
        float3 kS = fresnel;
        float3 kD = 1 - kS;
        kD *= 1.0f - metalness;
    
        float3 numerator = normalDistribution * geometry * fresnel;
        float denominator = 4.0f * max(dot(bumpNormal, -viewDirection), 0.0f) * max(dot(bumpNormal, gLightDirection), 0.0f) + gEpsilon;
        float3 specular = numerator / denominator;
    
        outgoingRadiance = (kD * albedo / gPI + specular) * radiance * max(dot(bumpNormal, gLightDirection), 0.0f);
    }
    
    float3 ambient = environment * albedo * ao;
    float3 color = ambient + outgoingRadiance;
    
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    
    PS_OUTPUT output = (PS_OUTPUT)0;
    output.Color0 = float4(color, 1.f);
    output.Color1 = float4(0.9f, 0.2f, 0.3f, 1.0f);
    
    return output;
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