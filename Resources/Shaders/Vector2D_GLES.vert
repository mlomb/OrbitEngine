#version 300 es

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

layout(std140) uniform PVMatrices{
	mat4 pr_matrix;
	mat4 vw_matrix;
};

out vec2 vs_position;
out vec2 vs_uv;

void main()
{
	gl_Position = pr_matrix * vw_matrix * vec4(position, 0, 1);

	vs_position = position;
	vs_uv = uv;
}