#pragma once
#include "Buffers/EBO.h"
#include "Buffers/VAO.h"
#include "Buffers/VBO.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	VBO m_Vbo;
	EBO m_Ebo;
	VAO m_Vao;

	static long long int free_idx;

public:
	Model(std::unique_ptr<float[]> vertices, unsigned int size_vertices,
		  std::unique_ptr<unsigned int[]> indices, unsigned int size_indices);
	~Model();

	void Bind();
	void Unbind();

	void ModelInit();
	VAO& getVAO();
	EBO& getEBO();
	unsigned int* getIndexReference();
	unsigned int  getIndexSize();
	VBO& getVBO(); 

};

