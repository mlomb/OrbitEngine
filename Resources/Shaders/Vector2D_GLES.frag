#version 300 es
precision mediump float;

layout(location = 0) out vec4 color;

in vec2 vs_position;
in vec2 vs_uv;

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

vec4 resolveTexture2DArr(float texid, vec2 uv) {
	// Waterfall code incoming
	//return vec4(texid / 4.0, 0.0,0.0,1.0);

	// PLEASE DO SOMETHING!
	if (texid == 1.0) return texture(textures[0], uv);
	else if (texid == 2.0) return texture(textures[1], uv);
	else if (texid == 3.0) return texture(textures[2], uv);
	else if (texid == 4.0) return texture(textures[3], uv);
	else if (texid == 5.0) return texture(textures[4], uv);
	else if (texid == 6.0) return texture(textures[5], uv);
	else if (texid == 7.0) return texture(textures[6], uv);
	else if (texid == 8.0) return texture(textures[7], uv);
	else if (texid == 9.0) return texture(textures[8], uv);
	else if (texid == 10.0) return texture(textures[9], uv);
	else if (texid == 11.0) return texture(textures[10], uv);
	else if (texid == 12.0) return texture(textures[11], uv);
	else if (texid == 13.0) return texture(textures[12], uv);
	else if (texid == 14.0) return texture(textures[13], uv);
	else if (texid == 15.0) return texture(textures[14], uv);
	return vec4(1, 0.5, 1, 1);
}


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
	vec2 pt = (style.paintMat * vec4(vs_position, 0, 1)).xy;
	float scissor = calcScissor(vs_position);
	
	float d = clamp((sdroundrect(pt, style.extent, style.radius) + style.feather*0.5) / style.feather, 0.0, 1.0);
	color = mix(style.innerColor, style.outerColor, d);

	if(style.texid > 0.0){
		vec2 pt = pt / style.extent;
		pt.y = 1.0f - pt.y;

		color *= resolveTexture2DArr(style.texid, pt);
	}
	
	if(texid > 0.0){
		color *= resolveTexture2DArr(texid, vs_uv);
	} else
		color *= strokeMask(vs_uv, strokeMul);

	//color *= scissor;
}
