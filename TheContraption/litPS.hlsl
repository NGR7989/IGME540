

#define MAX_SPECULAR_EXPONENT 256.0f

#include "ShaderInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float3 camPos;
	float roughness;
	float3 ambient;
	Light directionalLight1;
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
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering


	float3 lightDir = normalize(input.worldPosition - directionalLight1.position);
	float3 diffuse = saturate(dot(input.normal, lightDir));

	float3 baseColor = float3(1, 1, 1);
	float3 diffColor = (diffuse * directionalLight1.color * baseColor) + (ambient * baseColor));
	
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float3 V = normalize(input.worldPosition - camPos);
	float3 R = reflect(lightDir, input.normal);

	float spec = pow(saturate(dot(R, V)), specExponent);
	float3 light = diffColor * diffuse + spec;

	return float4(diffColor, 1);
}