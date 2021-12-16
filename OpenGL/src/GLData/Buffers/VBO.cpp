#include "VBO.h"
#include "GL/glew.h"


VBO::VBO(float* vertices)
	: verts(vertices), size(sizeof(vertices))
{

	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

VBO::~VBO()
{
	glDeleteBuffers(1, &RenderID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setArray(float* newArr)
{
}

float* VBO::getArray()
{
	return verts;
}

unsigned int VBO::getRenderID()
{
	return RenderID;
}

unsigned int VBO::getSize()
{
	return size;
}
