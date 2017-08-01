struct Input {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float texid : TEXID;
};

Texture2D textures[16];
SamplerState samplers[16];


float4 resolveTexture2DArr(float texid, float2 uv) {
	// Waterfall code incoming
	if (texid == 1) return textures[0].Sample(samplers[0], uv);
	else if (texid == 2) return textures[1].Sample(samplers[1], uv);
	else if (texid == 3) return textures[2].Sample(samplers[2], uv);
	else if (texid == 4) return textures[3].Sample(samplers[3], uv);
	else if (texid == 5) return textures[4].Sample(samplers[4], uv);
	else if (texid == 6) return textures[5].Sample(samplers[5], uv);
	else if (texid == 7) return textures[6].Sample(samplers[6], uv);
	else if (texid == 8) return textures[7].Sample(samplers[7], uv);
	else if (texid == 9) return textures[8].Sample(samplers[8], uv);
	else if (texid == 10) return textures[9].Sample(samplers[9], uv);
	else if (texid == 11) return textures[10].Sample(samplers[10], uv);
	else if (texid == 12) return textures[11].Sample(samplers[11], uv);
	else if (texid == 13) return textures[12].Sample(samplers[12], uv);
	else if (texid == 14) return textures[13].Sample(samplers[13], uv);
	else if (texid == 15) return textures[14].Sample(samplers[14], uv);
	else if (texid == 16) return textures[15].Sample(samplers[15], uv);
	return float4(1, 0, 1, 1);
}

float4 main(Input input) : SV_TARGET{
	float4 color = input.color;

	if (input.texid > 0)
		color *= resolveTexture2DArr(input.texid, input.uv);

	return color;
}