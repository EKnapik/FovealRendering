#include "Renderer.h"


using namespace DirectX;

Renderer::Renderer(Camera *camera, ID3D11Device* device, ID3D11DeviceContext *context,
	ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView)
{
	this->camera = camera;
	this->device = device;
	this->context = context;
	this->backBufferRTV = backBufferRTV;
	this->depthStencilView = depthStencilView;
}


Renderer::~Renderer()
{
	typedef std::map<std::string, Mesh*>::iterator mesh_type;
	for (mesh_type iterator = MeshDictionary.begin(); iterator != MeshDictionary.end(); iterator++) {
		delete iterator->second;
	}

	typedef std::map<std::string, SimplePixelShader*>::iterator pixel_type;
	for (pixel_type iterator = PixelShaderDictionary.begin(); iterator != PixelShaderDictionary.end(); iterator++) {
		delete iterator->second;
	}

	typedef std::map<std::string, SimpleVertexShader*>::iterator vertex_type;
	for (vertex_type iterator = VertexShaderDictionary.begin(); iterator != VertexShaderDictionary.end(); iterator++) {
		delete iterator->second;
	}
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
	SimpleVertexShader* vertexShader = GetVertexShader("default");
	SimplePixelShader* pixelShader = GetPixelShader(material->GetPixelShaderName());
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
	vertexShader->SetMatrix4x4("view", *camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", *camera->GetProjMat());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp;
	for (int i = 0; i < numEntities; i++) {
		vertexShader->SetMatrix4x4("world", *entities[i].GetWorldClean());
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
	SimpleVertexShader* vertexShader = GetVertexShader("default");
	SimplePixelShader* pixelShader = GetPixelShader(material->GetPixelShaderName());
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
	vertexShader->SetMatrix4x4("view", *camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", *camera->GetProjMat());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp;
	for (int i = 0; i < numEntities; i++) {
		vertexShader->SetMatrix4x4("world", *entities[i].GetWorldClean());
		vertexShader->CopyAllBufferData();

		meshTmp = entities[i].GetMesh();
		ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}
}


void Renderer::AddMesh(std::string name, Mesh* mesh)
{
	MeshDictionary.insert(std::pair<std::string, Mesh*>(name, mesh));
}

void Renderer::AddMesh(std::string name, std::string path)
{
	Mesh* mesh = new Mesh(path, device);
	MeshDictionary.insert(std::pair<std::string, Mesh*>(name, mesh));
}

Mesh* Renderer::GetMesh(std::string name)
{
	return MeshDictionary.at(name);
}

void Renderer::AddVertexShader(std::string name, std::wstring path)
{
	std::wstring debug = L"Debug/";
	debug += path;
	SimpleVertexShader* vertexShader = new SimpleVertexShader(device, context);

	if (!vertexShader->LoadShaderFile(debug.c_str()))
	{
		vertexShader->LoadShaderFile(path.c_str());
	}
	VertexShaderDictionary.insert(std::pair<std::string, SimpleVertexShader*>(name, vertexShader));
}

void Renderer::AddPixelShader(std::string name, std::wstring path)
{
	std::wstring debug = L"Debug/";
	debug += path;
	SimplePixelShader* pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(debug.c_str()))
	{
		pixelShader->LoadShaderFile(path.c_str());
	}
	PixelShaderDictionary.insert(std::pair<std::string, SimplePixelShader*>(name, pixelShader));
}

SimpleVertexShader * Renderer::GetVertexShader(std::string name)
{
	return VertexShaderDictionary.at(name);
}

SimplePixelShader * Renderer::GetPixelShader(std::string name)
{
	return PixelShaderDictionary.at(name);
}
