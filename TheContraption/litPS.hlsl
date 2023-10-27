

#define MAX_SPECULAR_EXPONENT 256.0f

#include "ShaderInclude.hlsli"

Texture2D SurfaceTexture : register(t0); // "t" registers for textures
Texture2D SpeculuarTexture : register(t1); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers

cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float3 camPos;
	float roughness;
	float2 uvOffset;
	float3 ambient;
	Light directionalLight1;
	Light directionalLight2;
	Light directionalLight3;
	Light spotLight1;
	Light spotLight2;
}

float2 GetUV(VertexToPixel input)
{
	return input.uv + uvOffset;
}

float3 GetSurfaceColor(VertexToPixel input)
{
	float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, GetUV(input)).rgb;
	return surfaceColor;
}

float GetSpec(VertexToPixel input)
{
	return SpeculuarTexture.Sample(BasicSampler, GetUV(input)).r;
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

	float3 diffColor = (diffuse * light.color * GetSurfaceColor(input)) + (ambient * GetSurfaceColor(input));

	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float3 V = normalize(input.worldPosition - camPos);
	float3 R = reflect(lightDir, input.normal);

	float spec = pow(saturate(dot(R, V)), specExponent) * GetSpec(input);

	return (diffColor * diffuse + spec);
}

float3 SpotLight(Light light, VertexToPixel input, float3 ambient, float roughness)
{
	float3 lightDir = normalize(light.position-input.worldPosition );
	float3 diffuse = saturate(dot(input.normal, lightDir));

	float3 diffColor = (diffuse * light.color * GetSurfaceColor(input)) + (ambient * GetSurfaceColor(input));

	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float3 V = normalize(input.worldPosition - camPos);
	float3 R = reflect(lightDir, input.normal);

	float spec = pow(saturate(dot(R, V)), specExponent) * GetSpec(input);

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
	//float3 lightDir = normalize(spotLight1.position - input.worldPosition);
	//float3 diffuse = saturate(dot(input.normal, lightDir));
	//return float4(diffuse, 1.0f);
	//float3 diffColor = (diffuse * light.color * GetSurfaceColor(input)) + (ambient * GetSurfaceColor(input));
	//return diffColor;

	// Dir lights 
	float3 light1 = DirLight(directionalLight1, input, ambient, roughness);
	float3 light2 = DirLight(directionalLight2, input, ambient, roughness);
	float3 light3 = DirLight(directionalLight3, input, ambient, roughness);

	// Point lights
	float3 light4 = SpotLight(spotLight1, input, ambient, roughness);
	float3 light5 = SpotLight(spotLight2, input, ambient, roughness);
	return float4(light4 + light5, 1);

	float3 totalLight = light1 + light2 + light3 + light4 + light5;
	return float4(totalLight, 1);
}