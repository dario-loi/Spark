#include "Model.h"

Model::Model(std::vector<float>&& vertices, std::vector<unsigned int>&& indices)
	: m_Vbo(std::move(vertices)), m_Ebo(std::move(indices))
{
}

Model::~Model() = default;

void Model::Bind() const
{
	m_Vao.Bind();
}

void Model::Unbind() const
{
	m_Vao.Unbind();

}

void Model::ModelInit() const
{
	m_Vao.Bind();
	m_Ebo.Bind();
	m_Vbo.Bind();
	m_Vao.init_VAO();
	m_Vao.Unbind();
	m_Ebo.Unbind();
	m_Vbo.Unbind();
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
