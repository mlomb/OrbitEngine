struct Input {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D GPosition : register(t0);
Texture2D GNormalRoughness : register(t1);
Texture2D GAlbedoMetallic : register(t2);

SamplerState samplerType : register(s0);

float4 main(Input input) : SV_TARGET {
	return float4(0,1,0,1);
	//return float4(GAlbedoMetallic.Sample(samplerType, input.uv).rgb, 1);
}