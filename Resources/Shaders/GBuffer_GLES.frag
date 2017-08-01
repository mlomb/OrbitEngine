#version 300 es
precision mediump float;

// Taken from https://github.com/jamieowen/glsl-blend/blob/master/overlay.glsl
float blendOverlay(float base, float blend) {
	return base < 0.5 ? (2.0*base*blend) : (1.0 - 2.0*(1.0 - base)*(1.0 - blend));
}

layout(location = 0) out vec4 GPosition;
layout(location = 1) out vec4 GNormalRoughness;
layout(location = 2) out vec4 GAlbedoMetallic;

in vec3 fragPos;
in vec2 texCoords;
in mat3 TBN;

uniform sampler2D map_albedo;
uniform sampler2D map_metallic;
uniform sampler2D map_roughness;
uniform sampler2D map_normals;

layout(std140) uniform MaterialUniforms{
	vec3 baseColor;
	float metallic;
	float roughness;

	float usingAlbedoMap;
	float usingNormalMap;
	float usingMetallicMap;
	float usingRoughnessMap;
};

#define PI 3.14159265359
#define GAMMA 2.2f

vec3 getMap(sampler2D map) {
	return texture(map, texCoords).rgb;
}

vec3 getGammaCorrectedMap(sampler2D map)
{
	return pow(getMap(map), vec3(GAMMA));
}

vec3 getMap(sampler2D map, float usingMap, vec3 tint) {
	return (1.0 - usingMap) * tint + usingMap * tint * getGammaCorrectedMap(map);
}

float getMapOverlay(sampler2D map, float usingMap, float base) {
	return (1.0 - usingMap) * base + usingMap * blendOverlay(base, getMap(map).r);
}

vec3 getNormal() {
	vec3 normal = TBN[2]; // TBN[2] = normalIn

	if (usingNormalMap > 0.0) {
		vec3 normalMap = getMap(map_normals) * 2.0 - 1.0;
		normal = normalize(TBN * normalMap);
	}

	return normal;
}

void main()
{
	GPosition = vec4(fragPos, 1.0);
	GNormalRoughness.rgb = getNormal();
	GNormalRoughness.a = getMapOverlay(map_roughness, usingRoughnessMap, roughness);
	GAlbedoMetallic.rgb = getMap(map_albedo, usingAlbedoMap, baseColor);
	GAlbedoMetallic.a = getMapOverlay(map_metallic, usingMetallicMap, metallic);
}