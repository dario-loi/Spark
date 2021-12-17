#include "Model.h"
Model::Model(float* vertices, unsigned int size_vertices, unsigned int* indices, 
	unsigned int size_indices)
	: m_Vbo(vertices, size_vertices), m_Ebo(indices, size_indices), m_Vao(0)
{
	
}

Model::~Model()
{
}

void Model::Bind()
{
	m_Vbo.Bind();
	m_Ebo.Bind();
	m_Vao.Bind();
}

void Model::Unbind()
{
	m_Vbo.Unbind();
	m_Ebo.Unbind();
	m_Vao.Unbind();
}

void Model::ModelInit()
{
	Bind();
	m_Vao.init_VAO();
	Unbind();
}

VAO& Model::getVAO()
{
	return m_Vao;
}

EBO& Model::getEBO()
{
	return m_Ebo;
}

unsigned int* Model::getIndexReference()
{
	return m_Ebo.getArray();
}

unsigned int Model::getIndexSize()
{
	return m_Ebo.getSize();
}

VBO& Model::getVBO()
{
	return m_Vbo;
}
