#include "Model.h"

Model::Model(std::vector<float>&& vertices, std::vector<unsigned int>&& indices)
	: m_Vbo(std::move(vertices)), m_Ebo(std::move(indices)), m_Vao(++Model::free_idx)
{
}

Model::~Model() = default;



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

std::vector<unsigned int> const& Model::getIndexReference() const
{
	return m_Ebo.getArray();
}


std::vector<float> const& Model::getBufferReference() const
{
	return m_Vbo.getArray();
}




size_t Model::getIndexSize() const
{
	return m_Ebo.getSize();
}

size_t Model::getBufferSize() const
{
	return m_Vbo.getSize();
}

VBO const& Model::getVBO() const
{
	return m_Vbo;
}

size_t Model::free_idx = 0;