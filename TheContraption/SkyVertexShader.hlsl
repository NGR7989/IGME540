#include "ShaderInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	// The data order does matter! Smaller data being placed first will
	// cause there to be spacing to be added by direct without telling us 
	// Since we need to know exactly how long items being passed are into
	// the shader this can cause a large issue for us.
	matrix viewMatrix;
	matrix projMatrix;
}


VertexToPixel_Sky main( VertexShaderInput input)
{
	VertexToPixel_Sky output;

	matrix viewNoTranslation = viewMatrix;
	viewNoTranslation._14 = 0;
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;

	matrix vp = mul(projMatrix, viewNoTranslation);
	output.screenPosition = mul(vp, float4(input.localPosition, 1.0f));
	
	output.screenPosition.z = output.screenPosition.w;
	output.sampleDir = input.localPosition;

	return output;




	// Old

	matrix wvp = mul(projMatrix, viewNoTranslation);
	output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
	output.screenPosition.z = output.screenPosition.w;

	// The vert is relatiive to the center of the cube 
	// and therefore can be used as a direction towards
	// its location 
	output.sampleDir = input.localPosition;

	return output;
}