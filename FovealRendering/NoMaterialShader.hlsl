
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float3 Position;
};


// Struct representing the data we expect to receive from earlier pipeline stages
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};


cbuffer lights : register(b0)
{
	PointLight pointLight;
};

// --------------------------------------------------------
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float pointAmount = saturate(dot(input.normal, normalize(pointLight.Position - input.worldPos)));
	
	float4 objColor = float4(.8, .8, .8, 1.0); // marble grey

	float4 finalColor = (pointAmount*pointLight.Color*objColor);

	return finalColor;
}