#pragma once
#include "VBO.h"
#include "GL/glew.h"

VBO::VBO(std::vector<float>&& vertices)
	: verts(std::move(vertices))
{
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
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

void VBO::setData(std::vector<float>&& newArr)
{
	verts = std::move(newArr);
}

