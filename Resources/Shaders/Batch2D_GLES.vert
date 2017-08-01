#version 300 es

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in float in_tid;

layout(std140) uniform Projection{
	mat4 pr_matrix;
};

out vec2 vs_uv;
out vec4 vs_color;
out float vs_tid;

void main()
{
    gl_Position = pr_matrix * vec4(in_position, 0.0, 1.0);
    vs_color = in_color;
    vs_uv = in_uv;
	vs_tid = in_tid;
}
