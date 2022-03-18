#pragma once
#include "EBO.h"
#include "GL/glew.h"

EBO::EBO( std::unique_ptr<unsigned int[]> indices, unsigned int length)
	: size(length)
{
	indx = std::move(indices);
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indx.get(), GL_STATIC_DRAW);
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

void EBO::setArray(std::unique_ptr<unsigned int[]> newArr)
{
	indx = std::move(newArr);
}
