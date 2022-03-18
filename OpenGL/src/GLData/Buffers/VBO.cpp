#pragma once
#include "VBO.h"
#include "GL/glew.h"

VBO::VBO(std::unique_ptr<float[]> vertices, unsigned int length)
	: size(length)
{
	this->verts = std::move(vertices);
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), verts.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

VBO::~VBO()
{
	glDeleteBuffers(1, &RenderID);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
}

void VBO::Unbind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setArray(std::unique_ptr<float[]> newArr)
{
	verts = std::move(newArr);
}

