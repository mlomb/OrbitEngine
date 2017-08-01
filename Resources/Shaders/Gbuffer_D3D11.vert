cbuffer CB_viewproj : register(b0)
{
    matrix pr_matrix;
    matrix vw_matrix;
};

cbuffer CB_model : register(b1)
{
    matrix ml_matrix;
};

struct Input {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
};

struct Output {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 fragPos : TEXCOORD1;
};

Output main(Input input) {
	Output output;

	float4x4 viewProj = mul(pr_matrix, vw_matrix);
	float4 worldPos = mul(ml_matrix, float4(input.position.xyz, 1));

	output.fragPos = float3(worldPos.x, worldPos.y, worldPos.z);
    output.position = mul(viewProj, worldPos);
	output.uv = input.uv;

	float3x3 model = (float3x3)ml_matrix;
	output.normal = normalize(mul(model, input.normal));
	output.tangent = normalize(mul(model, input.tangent));

	return output;
}