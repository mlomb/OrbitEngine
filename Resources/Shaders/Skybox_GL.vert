#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);

out vec3 fragPos;

void main() {
	mat4 inversed = inverse(pr_matrix * mat4(mat3(vw_matrix)));

	fragPos = normalize((inversed * vec4(position.xy, 1, 1)).xyz);
	//fragPos.y *= -1; // Flip
	gl_Position = vec4(position.xy, 1, 1);
}