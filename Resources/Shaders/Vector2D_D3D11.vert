cbuffer CB_viewproj : register(b0)
{
	matrix pr_matrix;
	matrix vw_matrix;
};

struct Input {
	float2 position : POSITION;
	float2 uv : TEXCOORD;
};

struct Output {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Output main(Input input) {
	Output output;

	float4x4 worldViewProj = mul(pr_matrix, vw_matrix);

	output.position = mul(worldViewProj, float4(input.position.x, input.position.y, 0, 1));
	output.uv = input.uv;

	return output;
}