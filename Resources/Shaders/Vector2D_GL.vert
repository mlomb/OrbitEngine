#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

layout(std140) uniform PVMatrices{
	mat4 pr_matrix;
	mat4 vw_matrix;
};

out DATA
{
	vec2 position;
	vec2 uv;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * vec4(position, 0, 1);

	vs_out.position = position;
	vs_out.uv = uv;
}