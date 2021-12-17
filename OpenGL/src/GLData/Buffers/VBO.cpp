#include "VBO.h"
#include "GL/glew.h"

VBO::VBO(float* vertices, unsigned int length)
	: verts(vertices), size(length), isBound(false)
{

	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

VBO::~VBO()
{
	delete[] verts;
	glDeleteBuffers(1, &RenderID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	isBound = true;
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	isBound = false;
}

void VBO::setArray(float* newArr)
{
	verts = newArr;
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
