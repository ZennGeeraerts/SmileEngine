/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Matrices : register( b0 )
{
    matrix ViewInverse;
}

cbuffer Material : register( b1 )
{
    bool UseAlbedoMap = false;
    float3 AlbedoValue = float3( 0.0f, 0.0f, 0.0f );

    bool UseNormalMap;

    bool UseMetalnessMap = false;
    float MetalnessValue = 0.0f;

    bool UseRoughnessMap = false;
    float RoughnessValue = 0.5f;

    bool UseAOMap = false;

    float3 AmbientColor = 1.f;
}

Texture2D AlbedoMap : register( t0 );
Texture2D NormalMap : register( t1 );
Texture2D MetalnessMap : register( t2 );
Texture2D RoughnessMap : register( t3 );
Texture2D AOMap : register( t4 );
TextureCube EnvironmentMap : register( t5 );

static const float3 gLightDirection = float3( -0.577f, -0.577f, 0.577f );
static const float3 gLightColor = float3( 1.0f, 1.0f, 1.0f );
static const float gLightIntensity = 3.f;

static const float gAOValue = 1.0f;

static const float gReflectionStrength = 0.9f;
static const float gRefractionStrength = 0.1f;
static const float gRefractionIndex = 0.3000f;

static const float gPI = 3.141592f;
static const float gEpsilon = 0.0001f;

SamplerState SamLinear : register( s0 );

struct PS_INPUT
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

float TrowbridgeReitzGGX( float3 normal, float3 halfVector, float roughness )
{
    float roughnessSqr = roughness * roughness;
    float roughnessSqrSqr = roughnessSqr * roughnessSqr;
    float NdotH = max( dot( normal, halfVector ), 0.0f );
    float NdotHSqr = NdotH * NdotH;

    float numerator = roughnessSqrSqr;
    float denomerator = ( NdotHSqr * ( roughnessSqrSqr - 1.f ) + 1.f );
    denomerator = gPI * denomerator * denomerator;

    return numerator / denomerator;
}

float3 Schlick( float cosTheta, float3 baseReflectivity )
{
    return baseReflectivity + ( 1 - baseReflectivity ) * pow( clamp( 1 - cosTheta, 0.0f, 1.0f ), 5 );
}

float3 SchlickGGX( float nDot, float roughness )
{
    float r = roughness + 1.0f;
    float k = ( r * r ) / 8.0f;

    float numerator = nDot;
    float denomerator = nDot * ( 1.0f - k ) + k;

    return numerator / denomerator;
}

float3 Smith( float3 normal, float3 viewDirection, float roughness )
{
    float NdotV = max( dot( normal, -viewDirection ), 0.0f );
    float NdotL = max( dot( normal, -gLightDirection ), 0.0f );
    return SchlickGGX( NdotL, roughness ) * SchlickGGX( NdotV, roughness );
}

PS_OUTPUT PSMain( PS_INPUT input ) : SV_TARGET
{
    float3 viewDirection = normalize( input.WorldPosition.xyz - ViewInverse[3].xyz );

    // Albedo
    float3 albedo = AlbedoValue;
    if ( UseAlbedoMap )
        albedo = pow( AlbedoMap.Sample( SamLinear, input.TexCoord ).rgb, 2.2f );

    // Normal
    float3 bumpNormal = input.Normal;
    if ( UseNormalMap )
    {
        float3 binormal = cross( input.Normal, input.Tangent );
        float3x3 localAxis = float3x3( input.Tangent, binormal, input.Normal );
        float3 sampledNormal = NormalMap.Sample( SamLinear, input.TexCoord ).rgb;
        sampledNormal = 2.f * sampledNormal - 1.f;
        bumpNormal = normalize( mul( sampledNormal, localAxis ) );
    }

    // AO
    float ao = gAOValue;
    if ( UseAOMap )
        ao = AOMap.Sample( SamLinear, input.TexCoord ).r;

    // Roughness
    float roughness = RoughnessValue;
    if ( UseRoughnessMap )
        roughness = RoughnessMap.Sample( SamLinear, input.TexCoord ).r;

    // Metalness
    float metalness = MetalnessValue;
    if ( UseMetalnessMap )
        metalness = MetalnessMap.Sample( SamLinear, input.TexCoord ).r;

    // Cube environment
    float3 reflected = reflect( -viewDirection, bumpNormal );
    float3 refracted = refract( -viewDirection, bumpNormal, gRefractionIndex );
    float3 environment = EnvironmentMap.Sample( SamLinear, reflected ) * gReflectionStrength +
                         EnvironmentMap.Sample( SamLinear, refracted ) * gRefractionStrength;
    environment = saturate( environment );
    environment = environment * ( 1 - roughness );

    float3 baseReflectivity = 0.04f;
    baseReflectivity = lerp( baseReflectivity, albedo, metalness );

    float3 radianceColor = lerp( gLightColor, albedo, metalness );
    float3 radiance = mul( radianceColor, gLightIntensity );

    // cooktorrence
    float3 outgoingRadiance = 0.0f;
    {
        float3 halfVector = normalize( -viewDirection + -gLightDirection );

        float normalDistribution = TrowbridgeReitzGGX( bumpNormal, halfVector, roughness );
        float geometry = Smith( bumpNormal, viewDirection, roughness );
        float fresnel = Schlick( max( dot( halfVector, -viewDirection ), 0.0f ), baseReflectivity );

        float3 kS = fresnel;
        float3 kD = 1 - kS;
        kD *= 1.0f - metalness;

        float3 numerator = normalDistribution * geometry * fresnel;
        float denominator =
            4.0f * max( dot( bumpNormal, -viewDirection ), 0.0f ) * max( dot( bumpNormal, -gLightDirection ), 0.0f ) +
            gEpsilon;
        float3 specular = numerator / denominator;

        outgoingRadiance =
            ( kD * albedo / gPI + specular ) * radiance * max( dot( bumpNormal, -gLightDirection ), 0.0f );
    }

    float3 ambient = environment * albedo * ao;
    float3 color = ambient + outgoingRadiance;

    color = color / ( color + float3( 1.0f, 1.0f, 1.0f ) );
    color = pow( color, float3( 1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f ) );

    PS_OUTPUT output = ( PS_OUTPUT )0;
    output.Color0 = float4( color, 1.f );
    output.Color1 = float4( 0.9f, 0.2f, 0.3f, 1.0f );

    return output;
}