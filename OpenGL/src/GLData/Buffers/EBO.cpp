#pragma once
#include "EBO.h"
#include "GL/glew.h"

EBO::EBO( std::vector<unsigned int>&& indices) noexcept
	:
	indx(std::move(indices))
{
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indx.size() * sizeof(unsigned int), indx.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &RenderID);
}

void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
}

void EBO::Unbind() const 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::setArray(std::vector<unsigned int>&& newArr)
{
	indx = std::move(newArr);
}
