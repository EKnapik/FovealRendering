#pragma once

#include "DXCore.h"
#include "Vertex.h"
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>


class Mesh {
public:
	Mesh(ID3D11Device* device);
	Mesh(Vertex *vertices, int numVert, int *indices, int numInd, ID3D11Device* device);
	Mesh(std::string fileName, ID3D11Device* device);
	~Mesh();

	void GenMesh(Vertex * vertices, int numVertices, int * indices, int numIndices, ID3D11Device * device);
	ID3D11Buffer* GetVertexBuffer() { return this->vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return this->indexBuffer; }
	int GetVertexCount() { return this->numVert; }
	int GetIndexCount() { return this->numInd; }

private:
	ID3D11Buffer* vertexBuffer = 0;
	ID3D11Buffer* indexBuffer = 0;
	int numVert = 0;
	int numInd = 0;

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
};

