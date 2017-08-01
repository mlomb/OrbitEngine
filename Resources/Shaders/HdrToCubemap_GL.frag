#version 330

uniform sampler2D environment;

in vec4 fragPos;

out vec4 color;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main() {
	vec2 uv = SampleSphericalMap(normalize(fragPos.xyz));
	color = vec4(texture(environment, uv).rgb, 1.0);
}