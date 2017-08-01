cbuffer CB_proj : register(b0)
{
    matrix pr_matrix;
};

struct Input {
	float2 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float texid : TEXID;
};

struct Output {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float texid : TEXID;
};

Output main(Input input) {
	Output output;

    output.position = mul(pr_matrix, float4(input.position.x, input.position.y, 0, 1));
	output.color = input.color;
	output.uv = input.uv;
	output.texid = input.texid;

	return output;
}
