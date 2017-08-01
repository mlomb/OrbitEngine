#version 330

layout(location = 0) out vec4 color;

in DATA
{
	vec4 color;
	vec2 uv;
	float texid;
} fs_in;

uniform sampler2D textures[16];

void main()
{
	vec4 fcolor = fs_in.color;
	
	if (fs_in.texid > 0.0) {
		int tid = int(fs_in.texid - 0.5);
		fcolor *= texture2D(textures[tid], fs_in.uv);
	}
	
	color = fcolor;
}