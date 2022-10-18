#include "Model.h"

spark::Model::Model(boost::uuids::uuid UUID_, std::string const& name_, std::vector<float>&& vertices, std::vector<unsigned int>&& indices)
	: UUID(UUID_), m_Vbo(std::move(vertices)), m_Ebo(std::move(indices)), name(name_)
{
}

void spark::Model::Bind() const
{
	m_Vao.Bind();
}

void spark::Model::Unbind() const
{
	m_Vao.Unbind();

}

void spark::Model::ModelInit() const
{
	m_Vao.Bind();
	m_Ebo.Bind();
	m_Vbo.Bind();
	m_Vao.init_VAO();
	m_Vao.Unbind();
	m_Ebo.Unbind();
	m_Vbo.Unbind();
}

VAO& spark::Model::getVAO()
{
	return m_Vao;
}

EBO& spark::Model::getEBO()
{
	return m_Ebo;
}

std::vector<unsigned int> const& spark::Model::getIndexReference() const
{
	return m_Ebo.getArray();
}


std::vector<float> const& spark::Model::getBufferReference() const
{
	return m_Vbo.getArray();
}

inline std::string spark::Model::getName() const
{
	return name;
}

inline const boost::uuids::uuid spark::Model::getUUID() const
{
	return UUID;
}

inline void spark::Model::setName(std::string const& name_)
{
	name = name_;
}


size_t spark::Model::getIndexSize() const
{
	return m_Ebo.getSize();
}

size_t spark::Model::getBufferSize() const
{
	return m_Vbo.getSize();
}

VBO const& spark::Model::getVBO() const
{
	return m_Vbo;
}
