// blendOverlay() taken from https://github.com/jamieowen/glsl-blend/blob/master/overlay.glsl
float blendOverlay(float base, float blend) { return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend)); }

struct Input {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 fragPos : TEXCOORD1;
};

struct Output
{
	float4 GPosition : SV_Target0;
	float4 GNormalRoughness : SV_Target1;
	float4 GAlbedoMetallic : SV_Target3;
};

Texture2D map_albedo : register(t0);
Texture2D map_metallic : register(t1);
Texture2D map_roughness : register(t2);
Texture2D map_normals : register(t3);

SamplerState sampler_albedo : register(s0);
SamplerState sampler_metallic : register(s1);
SamplerState sampler_roughness : register(s2);
SamplerState sampler_normals : register(s3);

cbuffer CB_material : register(b2)
{
    float4 baseColor;
    float metallic;
    float roughness;
};

float3 normalMap(float3 T, float3 N, float2 normals_uv){
    float3 B = -normalize(cross(N, T));
	float3x3 TBN = float3x3(T, B, N);
	TBN = transpose(TBN);
	float3 normalMap = map_normals.Sample(sampler_normals, normals_uv).rgb * 2.0 - 1.0;
	normalMap = normalize(mul(TBN, normalMap));
	return normalMap;
}

Output main(Input input) : SV_TARGET {
	Output output;

	output.GPosition = float4(input.fragPos, 1.0f);
	output.GNormalRoughness.rgb = normalMap(input.tangent, input.normal, input.uv);
	output.GNormalRoughness.a = blendOverlay(roughness, map_roughness.Sample(sampler_roughness, input.uv).r);
	output.GAlbedoMetallic.rgb = map_albedo.Sample(sampler_albedo, input.uv).rgb;
	output.GAlbedoMetallic.a = blendOverlay(metallic, map_metallic.Sample(sampler_metallic, input.uv).r);

    return output;
}