
cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float time;
}

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
	float2 uv				: TEXCOORD;
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


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float rA = gnoise(input.uv + float2(time, 0));
	float rB = gnoise(input.uv + float2(time / 0.5, -time));

	float col = rA + rB - 0.5f;
	return float4(col, col, col, 1.0);
}