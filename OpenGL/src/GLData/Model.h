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
	Model(std::vector<float>&& vertices, std::vector<unsigned int>&& indices);
	~Model();

	void Bind();
	void Unbind();

	void ModelInit();
	VAO& getVAO();
	EBO& getEBO();
	std::vector<unsigned int> const& getIndexReference() const;
	std::vector<float> const& getBufferReference() const;
	size_t  getIndexSize() const;
	size_t getBufferSize() const;
	VBO const& getVBO() const; 

	static size_t free_idx;

};

