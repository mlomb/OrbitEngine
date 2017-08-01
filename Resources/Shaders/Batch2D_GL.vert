#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in float texid;

layout(std140) uniform Projection {
	mat4 pr_matrix;
};

out DATA
{
	vec4 color;
	vec2 uv;
	float texid;
} vs_out;

void main()
{
	gl_Position = pr_matrix * vec4(position, 0, 1);

	vs_out.color = color;
	vs_out.uv = uv;
	vs_out.texid = texid;
}