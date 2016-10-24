
// Texture info
Texture2D gAlbedo			: register(t0);
Texture2D gNormal			: register(t1);
Texture2D gPosition			: register(t2);
SamplerState basicSampler	: register(s0);

// Struct representing the data we expect to receive from earlier pipeline stages
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;

};


// --------------------------------------------------------
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	return gPosition.Sample(basicSampler, input.uv); 

	return float4(0, 0, 0, 0);
}