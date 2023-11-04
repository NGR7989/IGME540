#ifndef __GGP_SHADER_INCLUDES__ // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES__


#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light
{
	int type;
	float3 directiton;
	float range;
	float3 position;
	float intensity;
	float3 color;
	float spotFalloff;
	float3 padding;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition	: SV_POSITION;
	float3 worldPosition	: POSITION;
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
};

float rand2(float2 n) { return frac(sin(dot(n, float2(12.9898, 4.1414))) * 43758.5453); }


float gnoise(float2 n) {
	const float2 d = float2(0.0, 1.0);
	float2  b = floor(n),
		f = smoothstep(d.xx, d.yy, frac(n));

	//float2 f = frac(n);
	//f = f*f*(3.0-2.0*f);

	float x = lerp(rand2(b), rand2(b + d.yx), f.x),
		y = lerp(rand2(b + d.xy), rand2(b + d.yy), f.x);

	return lerp(x, y, f.y);
}


#endif