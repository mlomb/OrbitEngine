#version 300 es
precision mediump float;

out vec4 fragmentColor;

in vec4 vs_color;
in vec2 vs_uv;
in float vs_tid;

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
	return vec4(1, 0.5, 1, 1);
}

void main()
{
	vec4 fcolor = vs_color;
	
	if (vs_tid > 0.0) {
		fcolor *= resolveTexture2DArr(vs_tid, vs_uv);
	}
	
	fragmentColor = fcolor;
}
