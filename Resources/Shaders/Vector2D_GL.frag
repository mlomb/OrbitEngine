#version 330

layout(location = 0) out vec4 color;

in DATA
{
	vec2 position;
	vec2 uv;
} fs_in;

struct PaintStyle {
	mat4 paintMat;
	vec4 innerColor;
	vec4 outerColor;
	vec2 extent;
	float radius;
	float feather;
	float texid;
	float pad1, pad2, pad3;
};

layout(std140) uniform Data{
	vec4 scissor;
	PaintStyle style;
	float texid;
	float strokeMul;
	float pad4, pad5;
};

uniform sampler2D textures[16];

float calcScissor(vec2 p) {
	vec2 pos = scissor.xy;
	vec2 size = scissor.zw;
	vec2 halfSize = size * 0.5f;

	vec2 sc = abs(p - pos - halfSize) - halfSize + 0.5;
	return 1.0f - length(max(sc, 0.0));
}

float strokeMask(vec2 uv, float strokeMul) {
	return min(1.0, (1.0 - abs(uv.x*2.0 - 1.0)) * strokeMul) * min(1.0, uv.y);
}

float sdroundrect(vec2 pt, vec2 ext, float rad) {
	vec2 ext2 = ext - vec2(rad, rad);
	vec2 d = abs(pt) - ext2;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - rad;
}

void main()
{
	vec2 pt = (style.paintMat * vec4(fs_in.position, 0, 1)).xy;
	float scissor = calcScissor(fs_in.position);
	
	float d = clamp((sdroundrect(pt, style.extent, style.radius) + style.feather*0.5) / style.feather, 0.0, 1.0);
	color = mix(style.innerColor, style.outerColor, d);

	if(style.texid > 0){
		vec2 pt = pt / style.extent;
		pt.y = 1.0f - pt.y;

		int tid = int(style.texid - 0.5);
		color *= texture2D(textures[tid], pt);
	}
	
	if(texid > 0){
		int tid = int(texid - 0.5);
		color *= texture2D(textures[tid], fs_in.uv);
	} else
		color *= strokeMask(fs_in.uv, strokeMul);

	color *= scissor;
}