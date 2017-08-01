#version 330 core

#define PI 3.14159265359
#define GAMMA 2.2f

vec3 GammaCorrection(vec3 color)
{
	return pow(color, vec3(1.0 / GAMMA));
}

uniform samplerCube cubeMap;
uniform float lod = 1.5;
uniform float exposure = 1;

in vec3 fragPos;
out vec4 out_Color;

void main() {
	vec3 color = textureLod(cubeMap, fragPos, lod).rgb;
	color = vec3(1.0) - exp(-color * exposure);

	out_Color = vec4(GammaCorrection(color), 1.0);
}