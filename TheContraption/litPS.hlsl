

#define MAX_SPECULAR_EXPONENT 256.0f

#include "ShaderInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float3 camPos;
	float roughness;
	float3 ambient;
	Light directionalLight1;
	Light directionalLight2;
	Light directionalLight3;
	Light spotLight1;
}

float Attenuate(Light light, float3 worldPos)
{
	float dist = distance(light.position, worldPos);
	float att = saturate(1.0f - (dist * dist / (light.range * light.range)));
	return att * att;
}

float3 DirLight(Light light, VertexToPixel input, float3 ambient, float roughness)
{
	float3 lightDir = normalize(-light.directiton);
	float3 diffuse = saturate(dot(input.normal, lightDir));

	float3 baseColor = float3(1, 1, 1);
	float3 diffColor = (diffuse * light.color * baseColor) + (ambient * baseColor);

	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float3 V = normalize(input.worldPosition - camPos);
	float3 R = reflect(lightDir, input.normal);

	float spec = pow(saturate(dot(R, V)), specExponent);

	return (diffColor * diffuse + spec);
}

float3 SpotLight(Light light, VertexToPixel input, float3 ambient, float roughness)
{
	float3 lightDir = normalize(light.position-input.worldPosition );
	float3 diffuse = saturate(dot(input.normal, lightDir));

	float3 baseColor = float3(1, 1, 1);
	float3 diffColor = (diffuse * light.color * baseColor) + (ambient * baseColor);

	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float3 V = normalize(input.worldPosition - camPos);
	float3 R = reflect(lightDir, input.normal);

	float spec = pow(saturate(dot(R, V)), specExponent);

	return (diffColor * diffuse + spec) * Attenuate(light, input.worldPosition);
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

	// Dir lights 
	float3 light1 = DirLight(directionalLight1, input, ambient, roughness);
	float3 light2 = DirLight(directionalLight2, input, ambient, roughness);
	float3 light3 = DirLight(directionalLight3, input, ambient, roughness);

	// Point lights
	float3 light4 = SpotLight(spotLight1, input, ambient, roughness);

	float3 totalLight = light1 + light2 + light3 + light4;
	return float4(totalLight, 1);
}