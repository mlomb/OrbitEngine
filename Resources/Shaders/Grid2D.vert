////////////////////////////////
//
// Pretty Grid by Timo Kinnunen 2017 -> https://www.shadertoy.com/view/XdjcDd
//
// Drag up/down to zoom.
//
// Based loosely on distance meter by cupe
// @ https://www.shadertoy.com/view/ldK3zD
//
// This shader is licensed under
// Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License 
// [ http://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US ].
//

//#define MINUS

const float TAU = 2.* 3.1415926535897932384626433832795;
const float grid4 = 2097152.0,grid3 = 131072.0,grid2 = 1024.0,grid1 = 64.0;

vec4 grid(vec2 f, vec3 pos, float rdy, float t) {
    float ff = min(iResolution.x,iResolution.y)/1024.0;
	vec4 distances = (abs(pos.xz)* TAU).xxyy;
	float referenceBase = log(20.*ff/ (t* pow(abs(rdy),.8)))/ log(10.);
	float nearestBase = floor(referenceBase);
	float partialBase = fract(referenceBase);
	const vec4 gain = vec4(grid4- grid3,grid1,grid3- grid2,grid2- grid1);
	const vec4 off = vec4(grid3,0,grid2,grid1);
	vec4 exponentialBase = partialBase* partialBase* gain+ off;
	vec4 bases = pow(vec4(10),nearestBase+ vec4(-2,1,-1,0));
	vec4 lx = pow(.5+ .5* cos(distances* bases.xyxy),exponentialBase.xyxy);
	vec4 ly = pow(.5+ .5* cos(distances* bases.zwzw),exponentialBase.zwzw);
	vec4 l4 = (1.- lx* vec4(1.- partialBase,partialBase,1.- partialBase,partialBase))* (1.- ly);
	vec2 l2 = l4.xy* l4.zw;
	float l1 = .30078125* (1.- l2.x* l2.y);

	return vec4(l1, l1, l1,1);
}

vec4 mainImageGrid(vec2 f) {
	float rcpResY = 1./ iResolution.y;
	vec2 uv = 2.0* rcpResY* f- vec2(iResolution.x* rcpResY,1);
	vec3 ro = vec3(0,exp2(128.* (sin(iGlobalTime* .002)+ step(10.,iMouse.x)*(2.* rcpResY* iMouse.y- 1.))),0);
	vec3 rd = normalize(vec3(uv.x,-1,uv.y));
	float t = ro.y/ -rd.y;
	vec3 pos = ro+ t* rd;
	return grid(f,pos,rd.y,t);
}
void mainImage(out vec4 fragColor, vec2 fragCoord) {
	fragColor = mainImageGrid(fragCoord);
    /*
    float divisions = 10.0;
    float thickness = 0.01;
    
    float x = step(fract(fragCoord.x / (1.0 / divisions)), thickness);
    float y = step(fract(fragCoord.y / (1.0 / divisions)), thickness);
    float g = x + y;
    fragColor = vec4(fragCoord, 1, 1);
    */
}