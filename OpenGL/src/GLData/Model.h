#pragma once
#include "Buffers/EBO.h"
#include "Buffers/VAO.h"
#include "Buffers/VBO.h"


class Model
{
private:
	VBO m_Vbo;
	EBO m_Ebo;
	VAO m_Vao;

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

