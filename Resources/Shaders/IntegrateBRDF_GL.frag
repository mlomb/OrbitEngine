#version 330 core

#define PI 3.14159265359
#define GAMMA 2.2f

// -------------- Hammersley --------------
// See http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
vec2 Hammersley2D(uint i, uint N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(i));
}

// -------------- GGX --------------
// References
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// http://graphicrants.blogspot.com.ar/2013/08/specular-brdf-reference.html
// http://tcantenot.com/portfolio/2015/04/28/towards-physically-based-rendering/

// Normal distribution function D
// Returns the ratio of microfacets aligned to h (the halfway)
float D_TrowbridgeReitzGGX(float NoH, float r2) {
	float r4 = r2 * r2;
	float NoH2 = NoH * NoH;
	float denom = PI * pow(NoH2 * (r4 - 1.0) + 1.0, 2.0);
	return r4 / denom;
}

// Geometric function G
// Retruns the probability of a microsurface to be visible from the light direction and view direction
float G_SchlickGGX(float NoL, float NoV, float r)
{
	float k = (r * r) / 2.0;
	float GL = NoL / (NoL * (1.0 - k) + k);
	float GV = NoV / (NoV * (1.0 - k) + k);
	return GL * GV;
}

// Fresnel function F
// Unreal Modification
vec3 F_Schlick(vec3 F0, float LoH)
{
	return F0 + (1.0 - F0) * exp2((-5.55473 * LoH - 6.98316) * LoH);
}

// -------------- IBL --------------
vec3 IBL(vec3 N, vec3 V, float roughness, float metallic, vec3 albedo, samplerCube env, sampler2D brdfLUT)
{
	const float ENVIRONMENT_MIPS = 5.0;

	float NoV = clamp(dot(N, V), 0.0, 1.0);
	vec3 R = reflect(-V, N);

	vec2 EnvBRDF = texture(brdfLUT, vec2(max(NoV, 0.0), roughness)).rg;
	vec3 dielectricColor = vec3(0.04);
	vec3 diffColor = albedo.rgb * (1.0 - metallic);
	vec3 specColor = mix(dielectricColor, albedo, metallic); // * specularIntensity

	vec3 PrefilteredColor = textureLod(env, R, roughness * ENVIRONMENT_MIPS).xyz;
	vec3 specIBL = PrefilteredColor * (specColor * EnvBRDF.x + EnvBRDF.y);

	return diffColor + specIBL;
}

// -------------- BRDF --------------
vec3 SpecularBRDF(vec3 F0, float roughness, vec3 N, vec3 H, vec3 L, vec3 V) {
	float NoL = clamp(dot(N, L), 0.0, 1.0);
	float NoV = clamp(dot(N, V), 0.0, 1.0);
	float NoH = clamp(dot(N, H), 0.0, 1.0);
	float LoH = clamp(dot(L, H), 0.0, 1.0);

	float r2 = roughness * roughness;
	r2 = max(r2, 2.0e-3); // Prevent zero

	float G = G_SchlickGGX(NoL, NoV, roughness);
	float D = D_TrowbridgeReitzGGX(NoH, r2);
	vec3 F = F_Schlick(F0, LoH);

	return NoL * G * D * F;

	return (D * F * G) / (4 * NoL * NoV);
}

vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N)
{
	float a = Roughness * Roughness; // Using Disney's approach r = r2
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);
	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;
	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 TangentX = normalize(cross(UpVector, N));
	vec3 TangentY = cross(N, TangentX);
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

/* This generates a Look-Up Texture for the envBRDF (roughness / NdotV) */
/* See Unreal notes */
vec2 IntegrateBRDF(float Roughness, float NoV)
{
	vec3 N = vec3(0.0, 0.0, 1.0);
	vec3 V;
	V.x = sqrt(1.0f - NoV * NoV); // sin
	V.y = 0;
	V.z = NoV;					  // cos
	float A = 0;
	float B = 0;
	const uint NumSamples = 1024u;
	for (uint i = 0u; i < NumSamples; i++)
	{
		vec2 Xi = Hammersley2D(i, NumSamples);
		vec3 H = ImportanceSampleGGX(Xi, Roughness, N);
		vec3 L = 2 * dot(V, H) * H - V;

		float NoL = clamp(L.z, 0.0, 1.0);
		float NoH = clamp(H.z, 0.0, 1.0);
		float VoH = clamp(dot(V, H), 0.0, 1.0);

		if (NoL > 0)
		{
			float G = G_SchlickGGX(NoV, NoL, Roughness);
			float G_Vis = G * VoH / (NoH * NoV);
			float Fc = pow(1 - VoH, 5);
			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	return vec2(A, B) / NumSamples;
}

// Second term of split-sum approach, see Unreal notes
vec3 PrefilterEnvMap(float Roughness, vec3 R, samplerCube environmentMap)
{
	vec3 N = R;
	vec3 V = R;
	vec3 PrefilteredColor = vec3(0);
	const uint NumSamples = 4096u * 4u;
	float TotalWeight = 0.0;

	for (uint i = 0u; i < NumSamples; i++)
	{
		vec2 Xi = Hammersley2D(i, NumSamples);
		vec3 H = ImportanceSampleGGX(Xi, Roughness, N);
		vec3 L = 2 * dot(V, H) * H - V;
		float NoL = clamp(dot(N, L), 0.0, 1.0);
		if (NoL > 0)
		{
			PrefilteredColor += texture(environmentMap, L).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}

in vec2 texCoords;
out vec4 color;

void main() {
	color = vec4(IntegrateBRDF(texCoords.y, texCoords.x), 0.0, 1.0);
}