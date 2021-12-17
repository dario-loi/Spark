#include "EBO.h"
#include "GL/glew.h"

EBO::EBO(unsigned int* indices,unsigned int length)
	: indx(indices), size(length), isBound(false)
{
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indx, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO()
{
	delete[] indx;
	glDeleteBuffers(1, &RenderID);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
	isBound = true;
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	isBound = false;
}

void EBO::setArray(unsigned int* newArr)
{
	indx = newArr;
}

unsigned int* EBO::getArray()
{
	return indx;
}

unsigned int EBO::getRenderID()
{
	return RenderID;
}

unsigned int EBO::getSize()
{
	return size;
}
