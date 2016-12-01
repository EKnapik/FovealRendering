// Struct representing a single vertex worth of data
struct VertexShaderInput
{ 
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

// Constant Buffer
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float  offsetX;  // 
	float  offsetY;  // 
};

// Struct representing the data we're sending down the pipeline
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	matrix worldViewProj = mul(mul(world, view), projection);

	VertexToPixel output = (VertexToPixel)0;
	// This should currently be directly in front of the camera
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	// Addjusting mask by offset
	output.position.x = output.position.x + offsetX;
	output.position.y = output.position.y + offsetY;


	output.uv = input.uv;

	return output;
}