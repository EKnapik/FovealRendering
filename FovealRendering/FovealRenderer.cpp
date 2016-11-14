#include "FovealRenderer.h"



FovealRenderer::FovealRenderer(Camera *camera, ID3D11Device* device, ID3D11DeviceContext *context,
	ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, int width, int height) :
	Renderer(camera, device, context, backBufferRTV, depthStencilView)
{
	windowHeight = height;
	windowWidth = width;

	HRESULT hr = S_OK;

	// Create the albedo texture.
	D3D11_TEXTURE2D_DESC descAlbedoTexture;
	ID3D11Texture2D* AlbedoTexture;
	ZeroMemory(&descAlbedoTexture, sizeof(descAlbedoTexture));
	descAlbedoTexture.Width = width;
	descAlbedoTexture.Height = height;
	descAlbedoTexture.MipLevels = 1;
	descAlbedoTexture.ArraySize = 1;
	descAlbedoTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descAlbedoTexture.SampleDesc.Count = 1;
	descAlbedoTexture.SampleDesc.Quality = 0;
	descAlbedoTexture.Usage = D3D11_USAGE_DEFAULT;
	descAlbedoTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descAlbedoTexture.CPUAccessFlags = 0;
	descAlbedoTexture.MiscFlags = 0;

	hr = device->CreateTexture2D(&descAlbedoTexture, NULL, &AlbedoTexture);

	if (FAILED(hr))
		printf("Error creating albedo texture.\n");

	// Create the albedo render target.
	D3D11_RENDER_TARGET_VIEW_DESC albedoRTVDesc;
	ZeroMemory(&albedoRTVDesc, sizeof(albedoRTVDesc));
	albedoRTVDesc.Format = descAlbedoTexture.Format;
	albedoRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	albedoRTVDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(AlbedoTexture, &albedoRTVDesc, &AlbedoRTV);

	if (FAILED(hr))
		printf("Error creating albedo RT.\n");


	// Create the albedo shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC albedoSRVDesc;
	albedoSRVDesc.Format = descAlbedoTexture.Format;
	albedoSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	albedoSRVDesc.Texture2D.MostDetailedMip = 0;
	albedoSRVDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(AlbedoTexture, &albedoSRVDesc, &AlbedoSRV);

	if (FAILED(hr))
		printf("Error creating albedo SRV.\n");

	// Don't need the actual texture anymore
	AlbedoTexture->Release();


	// Normal.

	// Create the normal texture.
	D3D11_TEXTURE2D_DESC descNormalTexture;
	ID3D11Texture2D* NormalTexture;
	ZeroMemory(&descNormalTexture, sizeof(descNormalTexture));
	descNormalTexture.Width = width;
	descNormalTexture.Height = height;
	descNormalTexture.MipLevels = 1;
	descNormalTexture.ArraySize = 1;
	descNormalTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descNormalTexture.SampleDesc.Count = 1;
	descNormalTexture.SampleDesc.Quality = 0;
	descNormalTexture.Usage = D3D11_USAGE_DEFAULT;
	descNormalTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descNormalTexture.CPUAccessFlags = 0;
	descNormalTexture.MiscFlags = 0;

	hr = device->CreateTexture2D(&descNormalTexture, NULL, &NormalTexture);

	if (FAILED(hr))
		printf("Error creating normal texture.\n");


	// Create the Normal render target.
	D3D11_RENDER_TARGET_VIEW_DESC normalRTVDesc;
	ZeroMemory(&normalRTVDesc, sizeof(normalRTVDesc));
	normalRTVDesc.Format = descNormalTexture.Format;
	normalRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	normalRTVDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(NormalTexture, &normalRTVDesc, &NormalRTV);

	if (FAILED(hr))
		printf("Error creating normal RT.\n");


	// Create the Normal shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC normalSRVDesc;
	normalSRVDesc.Format = descNormalTexture.Format;
	normalSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	normalSRVDesc.Texture2D.MostDetailedMip = 0;
	normalSRVDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(NormalTexture, &normalSRVDesc, &NormalSRV);

	if (FAILED(hr))
		printf("Error creating normal SRV.\n");

	// Don't need the actual normal texture
	NormalTexture->Release();

	// Depth.

	// Create the depth texture.
	D3D11_TEXTURE2D_DESC descPositionTexture;
	ID3D11Texture2D* PositionTexture;
	ZeroMemory(&descPositionTexture, sizeof(descPositionTexture));
	descPositionTexture.Width = width;
	descPositionTexture.Height = height;
	descPositionTexture.MipLevels = 1;
	descPositionTexture.ArraySize = 1;
	descPositionTexture.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	descPositionTexture.SampleDesc.Count = 1;
	descPositionTexture.SampleDesc.Quality = 0;
	descPositionTexture.Usage = D3D11_USAGE_DEFAULT;
	descPositionTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descPositionTexture.CPUAccessFlags = 0;
	descPositionTexture.MiscFlags = 0;

	hr = device->CreateTexture2D(&descPositionTexture, NULL, &PositionTexture);

	if (FAILED(hr))
		printf("Error creating depth texture.\n");


	// Create the depth render target.
	D3D11_RENDER_TARGET_VIEW_DESC PositionRTVDesc;
	ZeroMemory(&PositionRTVDesc, sizeof(PositionRTVDesc));
	PositionRTVDesc.Format = descPositionTexture.Format;
	PositionRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	PositionRTVDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(PositionTexture, &PositionRTVDesc, &PositionRTV);

	if (FAILED(hr))
		printf("Error creating depth RT.\n");


	// Create the depth shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC PositionSRVDesc;
	PositionSRVDesc.Format = descPositionTexture.Format;
	PositionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	PositionSRVDesc.Texture2D.MostDetailedMip = 0;
	PositionSRVDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(PositionTexture, &PositionSRVDesc, &PositionSRV);

	// Don't need the actual depth texture
	PositionTexture->Release();

	// create sampler
	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampleDesc, &simpleSampler);

	// Create addative blend state needed for light rendering
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&bd, &blendState);

	// Create raster state for rendering lights propperly
	D3D11_RASTERIZER_DESC ligtRastDesc = {};
	ligtRastDesc.FillMode = D3D11_FILL_SOLID;
	ligtRastDesc.CullMode = D3D11_CULL_FRONT;
	ligtRastDesc.DepthClipEnable = false;
	device->CreateRasterizerState(&ligtRastDesc, &lightRastState);

	// create stencil write

	// create stencil compare

	// Depth stencil desc for decal drawing
	// Create depth stencil state corresponding to step one of the article.
	D3D11_DEPTH_STENCIL_DESC dsDescFirstpass;
	dsDescFirstpass.DepthEnable = false;
	dsDescFirstpass.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDescFirstpass.DepthFunc = D3D11_COMPARISON_NEVER;

	// Stencil test parameters
	dsDescFirstpass.StencilEnable = true;
	dsDescFirstpass.StencilReadMask = 0xFF;
	dsDescFirstpass.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.

	// Keep original value on fail.
	dsDescFirstpass.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescFirstpass.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// Write to the stencil on pass.
	dsDescFirstpass.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	dsDescFirstpass.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	// Since we do not care about back-facing pixels, always keep original value.
	dsDescFirstpass.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescFirstpass.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescFirstpass.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescFirstpass.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	device->CreateDepthStencilState(&dsDescFirstpass, &writeMask);

	//Depth stencil desc for re-drawing the main scene, using the stencil as a mask
	D3D11_DEPTH_STENCIL_DESC dsDescSecondpass;

	// WRITE EQUAL SO IT WILL REPLACE THE PREVIOUS ONES WITHOUT ALTERING THE BUFFER
	dsDescSecondpass.DepthEnable = false;
	dsDescSecondpass.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescSecondpass.DepthFunc = D3D11_COMPARISON_EQUAL;

	dsDescSecondpass.StencilEnable = true;
	dsDescSecondpass.StencilReadMask = 0xFF;
	dsDescSecondpass.StencilWriteMask = 0xFF;

	// It does not matter what we write since we are not using the values after this step.
	// In other words, we are only using the values to mask pixels.
	dsDescSecondpass.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescSecondpass.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescSecondpass.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	// The stencil test passes if the passed parameter is equal to value in the buffer.
	dsDescSecondpass.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;

	// Again, we do not care about back-facing pixels.
	dsDescSecondpass.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescSecondpass.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescSecondpass.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescSecondpass.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	//I then create the DSS and set it with:
	device->CreateDepthStencilState(&dsDescSecondpass, &readMask);


}


FovealRenderer::~FovealRenderer()
{
	// Albedo
	AlbedoRTV->Release();
	AlbedoSRV->Release();

	// Normal
	NormalRTV->Release();
	NormalSRV->Release();

	// Depth
	PositionRTV->Release();
	PositionSRV->Release();

	simpleSampler->Release();
	blendState->Release();
	writeMask->Release();
	readMask->Release();
}


void FovealRenderer::Render(int eyePosX, int eyePosY, GameEntity* entities, int numEntities,
	ScenePointLight* pointLights, int numPointLights,
	SceneDirectionalLight* dirLights, int numDirLights)
{
	// Background color (Cornflower Blue)
	const float clearColor[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV, black);
	// clear all the render targets
	context->ClearRenderTargetView(AlbedoRTV, black);
	context->ClearRenderTargetView(NormalRTV, black);
	context->ClearRenderTargetView(PositionRTV, black);

	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Draw low resolution to gBuffer
	// Stencil draw sphere, cone or cylinder
	// Draw high resolution to buffers 
	// ReRender only foveal area with shading calculations

	gBufferRender(eyePosX, eyePosY, entities, numEntities);
	context->OMSetRenderTargets(1, &backBufferRTV, 0);
	pointLightRender(pointLights, numPointLights);
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	directionalLightRender(dirLights, numDirLights);
}


void FovealRenderer::DrawLowRes()
{
	ID3D11RenderTargetView* RTViews[3] = { AlbedoRTV, NormalRTV, PositionRTV };

	context->OMSetRenderTargets(3, RTViews, depthStencilView);

	// RENDER NORMALLY NOW
	// DrawOneMaterial();
}


// SETTING THIS AS A TEST
void FovealRenderer::DrawHighRes()
{
	SimpleVertexShader* vertexShader = GetVertexShader("quad");
	SimplePixelShader* pixelShader = GetPixelShader("quad");
	vertexShader->SetShader();
	pixelShader->SetShader();

	pixelShader->SetFloat3("cameraPosition", *camera->GetPos());
	vertexShader->CopyAllBufferData();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp = GetMesh("quad");
	ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
	DirectionalLight light;
	// Send light info to pixel shader
	light.AmbientColor = DirectX::XMFLOAT4(1, 0, 1, 1);
	light.DiffuseColor = DirectX::XMFLOAT4(0, 0, 0, 1);
	light.Direction = DirectX::XMFLOAT3(0, 0, 0);
	pixelShader->SetData("dirLight", &light, sizeof(DirectionalLight));
	pixelShader->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
	context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);

	// RESET STATES
	pixelShader->SetShaderResourceView("gAlbedo", 0);
	pixelShader->SetShaderResourceView("gNormal", 0);
	pixelShader->SetShaderResourceView("gPosition", 0);
	return;
}


void FovealRenderer::DrawFinal()
{

}

void FovealRenderer::gBufferRender(int eyePosX, int eyePosY, GameEntity *entities, int numEntities)
{
	ID3D11RenderTargetView* RTViews[3] = { AlbedoRTV, NormalRTV, PositionRTV };
	context->OMSetRenderTargets(3, RTViews, depthStencilView);

	// RENDER NORMALLY NOW ALL Low Res
	DrawMultipleMaterials(entities, numEntities);

	// Setup Write Mask
	context->OMSetDepthStencilState(writeMask, 0);
	// Render masking at position
	DrawMask(eyePosX, eyePosY);
	// Setup read Mask and rerender with high res geometry
	context->OMSetDepthStencilState(readMask, 1);
	DrawHighRes();
	// Reset depth stencil
	context->OMSetDepthStencilState(0, 0);
}


// Don't need a pixel shader
void FovealRenderer::DrawMask(int eyePosX, int eyePosY) {
	SimpleVertexShader* vertexShader = GetVertexShader("fovealMask");
	context->PSSetShader(0, 0, 0); // No pixel shader needed
	vertexShader->SetShader();

	// send constant data
	vertexShader->SetMatrix4x4("view", *camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", *camera->GetProjMat());
	vertexShader->SetInt("posX", eyePosX);
	vertexShader->SetInt("posY", eyePosY);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp = GetMesh("sphere");
	ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT3 dir = *camera->GetDir();
	DirectX::XMFLOAT3 pos = *camera->GetPos();
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(pos.x + (dir.x*3), pos.y + (dir.y*3), pos.z + (dir.z*3))));
	vertexShader->SetMatrix4x4("world", world);

	vertexShader->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
	context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
}


/// Only Render Sphere Lights
void FovealRenderer::pointLightRender(ScenePointLight* pointLights, int numLights)
{
	float factors[4] = { 1,1,1,1 };
	context->OMSetBlendState(blendState, factors, 0xFFFFFFFF);
	context->RSSetState(lightRastState);

	SimpleVertexShader* vertexShader = GetVertexShader("default");
	SimplePixelShader* pixelShader = GetPixelShader("sphereLight");
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Send G buffers to pixel shader
	pixelShader->SetSamplerState("basicSampler", simpleSampler);
	pixelShader->SetShaderResourceView("gAlbedo", AlbedoSRV);
	pixelShader->SetShaderResourceView("gNormal", NormalSRV);
	pixelShader->SetShaderResourceView("gPosition", PositionSRV);
	// send constant data
	vertexShader->SetMatrix4x4("view", *camera->GetViewMat());
	vertexShader->SetMatrix4x4("projection", *camera->GetProjMat());

	pixelShader->SetFloat3("cameraPosition", *camera->GetPos());
	pixelShader->SetFloat("width", windowWidth);
	pixelShader->SetFloat("height", windowHeight);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp = GetMesh("sphere");
	ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
	PointLight light;
	DirectX::XMFLOAT4X4 world;

	for (int i = 0; i < numLights; i++) {
		// Send light info to pixel shader
		light.Color = pointLights[i].Color;
		light.Position = pointLights[i].Position;
		light.Radius = pointLights[i].Radius.x;
		pixelShader->SetData("pointLight", &light, sizeof(PointLight));

		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(pointLights[i].Radius.x, pointLights[i].Radius.y, pointLights[i].Radius.z)
			* DirectX::XMMatrixTranslation(pointLights[i].Position.x, pointLights[i].Position.y, pointLights[i].Position.z)));
		vertexShader->SetMatrix4x4("world", world);

		pixelShader->CopyAllBufferData();
		vertexShader->CopyAllBufferData();

		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}

	// RESET STATES
	pixelShader->SetShaderResourceView("gAlbedo", 0);
	pixelShader->SetShaderResourceView("gNormal", 0);
	pixelShader->SetShaderResourceView("gPosition", 0);
	context->OMSetBlendState(0, factors, 0xFFFFFFFF);
	context->RSSetState(0);
	return;
}


void FovealRenderer::directionalLightRender(SceneDirectionalLight* dirLights, int numLights) {
	float factors[4] = { 1,1,1,1 };
	context->OMSetBlendState(blendState, factors, 0xFFFFFFFF);

	SimpleVertexShader* vertexShader = GetVertexShader("quad");
	SimplePixelShader* pixelShader = GetPixelShader("quad");
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Send G buffers to pixel shader
	pixelShader->SetSamplerState("basicSampler", simpleSampler);
	pixelShader->SetShaderResourceView("gAlbedo", AlbedoSRV);
	pixelShader->SetShaderResourceView("gNormal", NormalSRV);
	pixelShader->SetShaderResourceView("gPosition", PositionSRV);

	pixelShader->SetFloat3("cameraPosition", *camera->GetPos());
	vertexShader->CopyAllBufferData();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Mesh* meshTmp = GetMesh("quad");
	ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
	DirectionalLight light;
	for (int i = 0; i < numLights; i++) {
		// Send light info to pixel shader
		light.AmbientColor = dirLights[i].AmbientColor;
		light.DiffuseColor = dirLights[i].DiffuseColor;
		light.Direction = dirLights[i].Direction;
		pixelShader->SetData("dirLight", &light, sizeof(DirectionalLight));
		pixelShader->CopyAllBufferData();

		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}

	// RESET STATES
	pixelShader->SetShaderResourceView("gAlbedo", 0);
	pixelShader->SetShaderResourceView("gNormal", 0);
	pixelShader->SetShaderResourceView("gPosition", 0);
	context->OMSetBlendState(0, factors, 0xFFFFFFFF);
	context->RSSetState(0);
	return;
}


// There will be no transmissive materials
void FovealRenderer::DrawMultipleMaterials(GameEntity* gameEntitys, int numEntities)
{
	SimpleVertexShader* vertexShader = GetVertexShader("gBuffer");
	SimplePixelShader* pixelShader = GetPixelShader("gBuffer");
	vertexShader->SetShader();
	pixelShader->SetShader();

	if (numEntities == 0) return;

	for (int i = 0; i < numEntities; i++)
	{
		Material* material = gameEntitys[i].GetMaterial();
		
		// Send texture Info
		pixelShader->SetSamplerState("basicSampler", this->simpleSampler);
		pixelShader->SetShaderResourceView("diffuseTexture", material->GetTexture());
		//pixelShader->SetShaderResourceView("NormalMap", material->GetNormMap());

		// Send Geometry
		vertexShader->SetMatrix4x4("view", *camera->GetViewMat());
		vertexShader->SetMatrix4x4("projection", *camera->GetProjMat());
		pixelShader->SetFloat3("cameraPosition", *camera->GetPos());


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		Mesh* meshTmp;
		vertexShader->SetMatrix4x4("world", *gameEntitys[i].GetWorldClean());
		vertexShader->CopyAllBufferData();
		pixelShader->CopyAllBufferData();

		meshTmp = gameEntitys[i].GetMesh();
		ID3D11Buffer* vertTemp = meshTmp->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vertTemp, &stride, &offset);
		context->IASetIndexBuffer(meshTmp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshTmp->GetIndexCount(), 0, 0);
	}
}