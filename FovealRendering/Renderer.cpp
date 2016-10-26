#include "Renderer.h"


using namespace DirectX;

Renderer::Renderer(Camera *camera, ID3D11DeviceContext *context,
	ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView)
{
	this->camera = camera;
	this->context = context;
	this->backBufferRTV = backBufferRTV;
	this->depthStencilView = depthStencilView;
}


Renderer::~Renderer()
{
}

void Renderer::Render(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue)
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}


void Renderer::DrawOneMaterial(GameEntity *entities, int numEntities, float deltaTime, float totalTime) {
	// Background color (Cornflower Blue)
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	if (numEntities == 0) return;
	Material* material = entities[0].GetMaterial();
	SimpleVertexShader* vertexShader = material->GetVertexShader();
	SimplePixelShader* pixelShader = material->GetPixelShader();
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Send texture Info
	pixelShader->SetSamplerState("Sampler", material->GetSampler());
	pixelShader->SetShaderResourceView("Texture", material->GetTexture());
	pixelShader->SetShaderResourceView("NormalMap", material->GetNormMap());

	// Send Lighting Info
	struct PointLight pointLight = PointLight();
	pointLight.Color = XMFLOAT4(0.8, 0.8, 0.1, 1.0);
	//pointLight.Position = XMFLOAT3(3.0*sin(totalTime), 2.0, 3.0*cos(totalTime));
	pointLight.Position = XMFLOAT3(3.0*sin(34), 3.0, 3.0*cos(34));
	pixelShader->SetData("pointLight", &pointLight, sizeof(PointLight));

	pixelShader->CopyAllBufferData();

	// Send Geometry
	vertexShader->SetMatrix4x4("view", camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", camera->GetProjMat());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp;
	for (int i = 0; i < numEntities; i++) {
		vertexShader->SetMatrix4x4("world", entities[i].GetWorldClean());
		vertexShader->CopyAllBufferData();

		meshTmp = entities[i].GetMesh();
		ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}

}

void Renderer::DrawNoMaterial(GameEntity *entities, int numEntities, float deltaTime, float totalTime) {
	// Background color (Cornflower Blue)
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	if (numEntities == 0) return;
	Material* material = entities[0].GetMaterial();
	SimpleVertexShader* vertexShader = material->GetVertexShader();
	SimplePixelShader* pixelShader = material->GetPixelShader();
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Send Lighting Info
	struct PointLight pointLight = PointLight();
	pointLight.Color = XMFLOAT4(0.8, 0.8, 0.1, 1.0);
	//pointLight.Position = XMFLOAT3(3.0*sin(totalTime), 2.0, 3.0*cos(totalTime));
	pointLight.Position = XMFLOAT3(3.0*sin(34), 3.0, 3.0*cos(34));
	pixelShader->SetData("pointLight", &pointLight, sizeof(PointLight));

	pixelShader->CopyAllBufferData();

	// Send Geometry
	vertexShader->SetMatrix4x4("view", camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", camera->GetProjMat());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp;
	for (int i = 0; i < numEntities; i++) {
		vertexShader->SetMatrix4x4("world", entities[i].GetWorldClean());
		vertexShader->CopyAllBufferData();

		meshTmp = entities[i].GetMesh();
		ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}
}


