#include "ShaderInclude.hlsli"

// Allows us to send in data through C++ 
cbuffer ExternalData : register(b0)
{
	// We are defining the order that these data structures
	// are being stored within the shader

	// The data order does matter! Smaller data being placed first will
	// cause there to be spacing to be added by direct without telling us 
	// Since we need to know exactly how long items being passed are into
	// the shader this can cause a large issue for us.
	matrix world; // Equivelent to 4x4 
	matrix viewMatrix;
	matrix projMatrix;
	matrix worldInvTranspose;
}


// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{ 
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 localPosition	: POSITION;     // XYZ position
	float3 normal			: NORMAL;
	float4 uv				: TEXCOORD;   
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;
	
	// Multiply the three matrices together first
	matrix wvp = mul(projMatrix, mul(viewMatrix, world));
	output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));

	output.uv = input.uv;

	//output.normal = mul((float3x3)worldInvTranspose, input.normal); // Perfect
	output.normal = input.normal; //mul((float3x3)world, input.normal);
	output.worldPosition = mul(world, float4(input.localPosition, 1.0f)).xyz;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}