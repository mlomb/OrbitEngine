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

	output.position = float4(input.position, 0, 1);
	output.uv = input.uv;

	return output;
}