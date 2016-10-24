
// Texture info
Texture2D diffuseTexture	: register(t0);
Texture2D NormalMap			: register(t1);
SamplerState basicSampler	: register(s0);

// Struct representing the data we expect to receive from earlier pipeline stages
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};


// --------------------------------------------------------
// --------------------------------------------------------
struct GBufferOutput
{
	float4 Color : COLOR0;
	float4 Normal : COLOR1;
	float4 Depth: COLOR2;
};

GBufferOutput main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	GBufferOutput output;
	// Set the diffuse albedo for the geometry
	output.Color.rgb = diffuseTexture.Sample(basicSampler, input.uv).rgb;
	output.Color.a = 1.0; // make sure there is full color for this object

	// Output the normal in [0, 1] space.
	output.Normal.rgb = 0.5f * (input.normal + 1.0f);
	// output.Normal.rgb = input.normal;
	// could store the specular component within this normal
	output.Normal.a = 1.0;

	// Set the depth
	output.Depth = float4(input.worldPos, 1.0);

	return output;
}