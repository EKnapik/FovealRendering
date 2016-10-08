
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

// Texture info
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
SamplerState Sampler	: register(s0);

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
	
	// Normal Mapping
	float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).rgb * 2 - 1;
	float3 T = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
	float3x3 TBN = float3x3(T, cross(T, input.normal), input.normal);
	input.normal = normalize(mul(normalFromMap, TBN));

	float pointAmount = saturate(dot(input.normal, normalize(pointLight.Position - input.worldPos)));
	
	float4 textureColor = Texture.Sample(Sampler, input.uv);

	float4 finalColor = (pointAmount*pointLight.Color*textureColor);

	return finalColor;
}