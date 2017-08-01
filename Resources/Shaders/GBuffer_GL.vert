#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 texCoordsIn;
layout(location = 3) in vec3 tangentIn;

layout(std140) uniform PVMatrices{
	mat4 pr_matrix;
	mat4 vw_matrix;
};
layout(std140) uniform MMatrix{
	mat4 ml_matrix;
};

out vec3 fragPos;
out vec2 texCoords;
out mat3 TBN;

void main()
{
	vec4 worldPos = ml_matrix * vec4(position, 1);
	fragPos = worldPos.xyz;
	gl_Position = pr_matrix * vw_matrix * worldPos;

	texCoords = texCoordsIn;

	mat3 model = mat3(ml_matrix);
	vec3 T = normalize(model * tangentIn);
	vec3 N = normalize(model * normalIn);
	vec3 B = normalize(cross(N, T));

	TBN = mat3(T, B, N);
}