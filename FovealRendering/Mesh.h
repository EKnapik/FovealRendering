#pragma once

#include "DXCore.h"
#include "Vertex.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>

class Mesh {
public:
	Mesh(Vertex *vertices, int numVert, int *indices, int numInd, ID3D11Device* device);
	Mesh(char* objFile, ID3D11Device* device);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer = 0;
	ID3D11Buffer* indexBuffer = 0;
	int numVert = 0;
	int numInd = 0;

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
};

